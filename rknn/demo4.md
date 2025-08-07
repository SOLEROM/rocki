# wav2txt

* ref https://github.com/airockchip/rknn_model_zoo/tree/main/examples/wav2vec2

## (host) dw

```
cd /home/user/shared/rknn_model_zoo/examples/wav2vec2/model
bash download_model.sh

```

## (host) convert

```
conda activate rknn

cd ../python/
python convert.py ../model/wav2vec2_base_960h_20s.onnx rk3588

```

```
examples/wav2vec2/model/
|-- download_model.sh
|-- test.wav
|-- wav2vec2_base_960h_20s.onnx
`-- wav2vec2_base_960h_20s.rknn
```



## (host) build app

```
export GCC_COMPILER=/opt/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu

cd /home/user/shared/rknn_model_zoo

./build-linux.sh -t rk3588 -a aarch64 -d wav2vec2


```


```
install/rk3588_linux_aarch64/rknn_wav2vec2_demo/
|-- lib
|   |-- librga.so
|   `-- librknnrt.so
|-- model
|   |-- test.wav
|   `-- wav2vec2_base_960h_20s.rknn
`-- rknn_wav2vec2_demo


```


## (target) run test

* copy app to target 

```
cd ~/proj/rknn_wav2vec2_demo

./rknn_wav2vec2_demo model/wav2vec2_base_960h_20s.rknn  a.wav 


```

* example

```
resample_audio: 48000 HZ -> 16000 HZ 
-- read_audio & convert_channels & resample_audio use: 28.936001 ms
-- audio_preprocess use: 1.914000 ms
model input num: 1, output num: 1
input tensors:
  index=0, name=input, n_dims=2, dims=[1, 320000], n_elems=320000, size=640000, fmt=UNDEFINED, type=FP16, qnt_type=AFFINE, zp=0, scale=1.000000
output tensors:
  index=0, name=output, n_dims=3, dims=[1, 999, 32], n_elems=31968, size=63936, fmt=UNDEFINED, type=FP16, qnt_type=AFFINE, zp=0, scale=1.000000
-- init_wav2vec2_model use: 779.271973 ms
-- inference_wav2vec2_model use: 3451.257080 ms

Wav2vec2 output: HELLO SIR CAN YOU FIX MY COMPUTEL I NEED TEUR CHAR BOOKS 

Real Time Factor (RTF): 3.451 / 20.000 = 0.173
```