
# retinaface_facenet

git clone https://github.com/LuckfoxTECH/luckfox_pico_rknn_example.git

### convert onnx to rknn

```
(RKNN-Toolkit2) user@RKNN4ALL:~/shared/1103/luckfox_pico_rknn_example/scripts/luckfox_onnx_to_rknn/convert$ convert.py ../model/retinaface.onnx ../dataset/retinaface_dataset.txt ../model/retinaface.rknn Retinaface

```


### build app with glibc

```
cd ~/shared/1103/luckfox_pico_rknn_example

export GLIBC_COMPILER=/home/user/shared/compilers/armhf/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-

./build.sh 

You selected: glibc
1) luckfox_pico_retinaface_facenet
2) luckfox_pico_retinaface_facenet_spidev
3) luckfox_pico_yolov5
Enter your choice [1-3]: 1
```


### 


export LUCKFOX_SDK_PATH=/home/user/shared/luckfox-pico