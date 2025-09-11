# About


## convert onnx to rknn

* run modes

```

verbose to use  RKNN(verbose=True) and see cpu/npu device info
> python convert.py  trans1.onnx rk3566 fp -v -o aaa.rknn


with cal
> python convert.py  trans1.onnx rk3566 i8 --dataset ./cal/dataset.txt  -o aaa.rknn

```