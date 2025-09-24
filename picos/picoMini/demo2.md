# yolov5

conda activate RKNN-Toolkit2
(RKNN-Toolkit2) user@RKNN4ALL:~/shared/zooLatest/rknn_model_zoo/examples/yolov5/python$ python  convert.py ../model/yolov5s_relu.onnx rv1103




export GCC_COMPILER=/home/user/shared/compilers/uclibcgnueabihf/arm-rockchip830-linux-uclibcgnueabihf/bin/arm-rockchip830-linux-uclibcgnueabihf

(base) user@RKNN4ALL:~/shared/zooLatest/rknn_model_zoo$ ./build-linux.sh -t  rv1103 -a armv7l -d yolov5




## run

```
RkLunch-stop.sh
./rknn_yolov5_demo model/yolov5.rknn model/bus.jpg 

```

