#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
convert_generic.py — ONNX → RKNN (model-agnostic)

Quick start
-----------
# FP (FP16 on NPU), let shapes be inferred from ONNX:
python convert_generic.py model.onnx rk3588

# INT8 quantization with calibration set:
python convert_generic.py model.onnx rk3588 i8 --dataset ./cal/dataset.txt -o model_i8.rknn

# Override/define inputs (repeatable). Format: --input <name>:D0,D1,D2,...
# Example for a typical image model (NCHW):
python convert_generic.py model.onnx rk3588 fp \
  --input input:1,3,224,224 -o model_fp.rknn

# For models with multiple inputs:
python convert_generic.py model.onnx rk3588 u8 \
  --input image:1,3,640,640 --input tokens:1,128 -o multi_in.rknn

Arguments
---------
positional:
  onnx_path     Path to ONNX file
  platform      One of:
                rk3562 rk3566 rk3568 rk3576 rk3588
                rv1103 rv1106 rk1808 rv1109 rv1126
  dtype         fp | i8 | u8   (default: fp → FP16 on NPU)

options:
  -o, --output              Output .rknn file (default: model.rknn)
  --dataset PATH            Calibration dataset.txt (required for i8/u8)
  --opt N                   RKNN optimization_level (default: 0)
  --acc                     Run accuracy_analysis() with a random tensor per input
  --run                     After export, init host runtime and run a dummy inference
  -v, --verbose             Verbose RKNN logs
  --input NAME:SHAPE        Override/add an input, e.g. "input:1,3,224,224". Repeatable.

Notes
-----
• If an ONNX input has dynamic dims, they are replaced with 1 unless overridden via --input.
• For quantization (i8/u8), you must provide --dataset (a text file listing sample paths).
• This script does not assume any data layout; it just forwards names/shapes to RKNN.
• If your ONNX lacks static input shapes, use --input to specify them.
"""

from __future__ import annotations
import sys, os, argparse, re
from typing import List, Tuple, Dict, Optional
import numpy as np

# RKNN Toolkit 2
from rknn.api import RKNN

try:
    import onnx
except ImportError:
    print("ERROR: onnx is required. Install with: pip install onnx", file=sys.stderr)
    sys.exit(1)

VALID_PLATFORMS = {
    'rk3562','rk3566','rk3568','rk3576','rk3588',
    'rv1103','rv1106','rk1808','rv1109','rv1126'
}

def parse_cli():
    p = argparse.ArgumentParser(
        formatter_class=argparse.RawTextHelpFormatter,
        description="Generic ONNX → RKNN converter (no model-specific assumptions)."
    )
    p.add_argument('onnx_path', help='Path to ONNX model')
    p.add_argument('platform', help='RKNN target platform (e.g., rk3588)')
    p.add_argument('dtype', nargs='?', default='fp', choices=['fp','i8','u8'],
                   help='Inference dtype: fp (default), i8, u8')
    p.add_argument('-o', '--output', default='model.rknn', help='Output RKNN path')
    p.add_argument('--dataset', default=None, help='Calibration dataset.txt (required for i8/u8)')
    p.add_argument('--opt', type=int, default=0, help='optimization_level (default: 0)')
    p.add_argument('--acc', action='store_true', help='Run accuracy_analysis() with random inputs')
    p.add_argument('--run', action='store_true', help='Init host runtime and run a dummy inference')
    p.add_argument('-v', '--verbose', action='store_true', help='Verbose RKNN logs')
    p.add_argument('--input', action='append', default=[],
                   help='Override/add input as NAME:Dim0,Dim1,... (repeatable)')
    args = p.parse_args()

    platform = args.platform.lower()
    if platform not in VALID_PLATFORMS:
        p.error(f"Unknown platform '{args.platform}'. Valid: {', '.join(sorted(VALID_PLATFORMS))}")

    if args.dtype in ('i8','u8') and not args.dataset:
        p.error("Quantization selected (i8/u8) but --dataset not provided.")

    return args

def load_onnx_inputs(path: str) -> List[Tuple[str, List[Optional[int]]]]:
    """Return [(name, [d0,d1,...])] where dynamic/None dims remain None."""
    model = onnx.load(path)
    g = model.graph
    inputs = []
    # Filter out initializers (weights) to keep only real inputs
    initializer_names = {init.name for init in g.initializer}
    for inp in g.input:
        if inp.name in initializer_names:
            continue
        shape = []
        if hasattr(inp.type, 'tensor_type') and inp.type.tensor_type.HasField('shape'):
            for d in inp.type.tensor_type.shape.dim:
                if d.dim_value > 0:
                    shape.append(int(d.dim_value))
                else:
                    # dim_param or zero → treat as dynamic
                    shape.append(None)
        else:
            shape = []  # unknown
        inputs.append((inp.name, shape))
    return inputs

def parse_input_overrides(overrides: List[str]) -> Dict[str, List[int]]:
    """
    Parse --input flags: "name:1,3,224,224"
    Return dict name -> [dims...]
    """
    out = {}
    for spec in overrides:
        if ':' not in spec:
            raise ValueError(f"--input expects NAME:Dim0,Dim1,... got '{spec}'")
        name, dims_str = spec.split(':', 1)
        name = name.strip()
        dims = [int(x) for x in re.split(r'[,\s]+', dims_str.strip()) if x]
        if not dims:
            raise ValueError(f"--input '{spec}': no dims parsed")
        out[name] = dims
    return out

def coalesce_shapes(
    onnx_inputs: List[Tuple[str, List[Optional[int]]]],
    overrides: Dict[str, List[int]]
) -> Tuple[List[str], List[List[int]]]:
    """
    Merge ONNX-declared shapes with user overrides.
    Replace dynamic/None dims with 1 if not overridden.
    Preserve input ordering from the ONNX graph.
    """
    names: List[str] = []
    sizes: List[List[int]] = []
    for name, shp in onnx_inputs:
        if name in overrides:
            final = overrides[name]
        else:
            final = [(d if isinstance(d, int) and d > 0 else 1) for d in (shp or [])]
            if not final:
                # Worst case: ONNX has no shape info at all → require override
                raise ValueError(
                    f"Input '{name}' has no static shape in ONNX; "
                    f"please provide --input {name}:D0,D1,..."
                )
        names.append(name)
        sizes.append(final)
    # Also allow overrides for inputs that aren't explicitly listed as graph inputs
    for extra, dims in overrides.items():
        if extra not in names:
            names.append(extra)
            sizes.append(dims)
    return names, sizes

def print_io_summary(names: List[str], sizes: List[List[int]]):
    print("Model inputs:")
    for n, s in zip(names, sizes):
        print(f"  • {n}: {s}")
    print()

def random_tensor(shape: List[int], dtype=np.float32):
    return (np.random.randn(*shape)).astype(dtype)

def main():
    args = parse_cli()

    # 1) Collect inputs
    onnx_inputs = load_onnx_inputs(args.onnx_path)
    overrides = parse_input_overrides(args.input) if args.input else {}
    try:
        input_names, input_sizes = coalesce_shapes(onnx_inputs, overrides)
    except ValueError as e:
        print(f"ERROR: {e}", file=sys.stderr)
        sys.exit(2)

    print_io_summary(input_names, input_sizes)

    # 2) RKNN session
    rknn = RKNN(verbose=args.verbose)

    print("[1/5] config()")
    # Minimal defaults; tweak as needed (mean/std, reorder, etc.)
    rknn.config(
        target_platform=args.platform,
        optimization_level=args.opt
        # You can add mean_values/std_values/reorder_channel if your inputs are images.
    )

    print("[2/5] load_onnx()", args.onnx_path)
    ret = rknn.load_onnx(
        model=args.onnx_path,
        inputs=input_names,
        input_size_list=input_sizes
    )
    if ret != 0:
        print("load_onnx failed.", file=sys.stderr)
        sys.exit(3)

    # 3) Build
    do_quant = (args.dtype in ('i8','u8'))
    print(f"[3/5] build()  {'with quantization' if do_quant else '(FP)'}")
    ret = rknn.build(
        do_quantization=do_quant,
        dataset=args.dataset if do_quant else None
    )
    if ret != 0:
        print("build failed.", file=sys.stderr)
        sys.exit(4)

    # 4) Optional: accuracy_analysis with random tensors (simulator)
    if args.acc:
        print("[4/5] accuracy_analysis() with random inputs")
        # Create temporary .npy files for each input (RKNN expects file paths)
        import tempfile, atexit
        tmp_paths = []
        for name, shape in zip(input_names, input_sizes):
            arr = random_tensor(shape, np.float32)
            f = tempfile.NamedTemporaryFile(suffix='.npy', delete=False)
            np.save(f.name, arr)
            f.close()
            tmp_paths.append(f.name)
            atexit.register(lambda p=f.name: os.path.exists(p) and os.remove(p))
            print(f"  - {name} <- {f.name}  (shape {arr.shape})")
        try:
            rknn.accuracy_analysis(inputs=tmp_paths, target=None)
        except Exception as e:
            print(f"⚠ accuracy_analysis skipped ({e})")

    # 5) Export
    print(f"[5/5] export_rknn() → {args.output}")
    ret = rknn.export_rknn(args.output)
    if ret != 0:
        print("export_rknn failed.", file=sys.stderr)
        sys.exit(5)

    # Optional: init host runtime & run a dummy inference
    if args.run:
        print("[extra] init_runtime(target=None) — host CPU")
        if rknn.init_runtime(target=None) != 0:
            print("init_runtime failed.", file=sys.stderr)
            sys.exit(6)
        # Build input dict (ordered array list is also fine)
        inputs_np = [random_tensor(s, np.float32) for s in input_sizes]
        outs = rknn.inference(inputs=inputs_np)
        print(f"Host inference OK. Outputs: {len(outs)}; first shape: {outs[0].shape if outs else 'n/a'}")

    print(f"✔ RKNN saved at {args.output}")
    rknn.release()

if __name__ == '__main__':
    main()
