

* get the code:

wget https://github.com/zifeng-radxa/rknn_model_zoo/releases/download/yolov5s_rknn/yolov5s_rk3566.rknn
git clone https://github.com/airockchip/rknn_model_zoo.git

* app code changes:
https://docs.radxa.com/en/zero/zero3/app-development/rknn_toolkit_lite2_yolov5

* deps:
    [enable npu](./enableNPU.md)
    pip3 install opencv-python-headless
    rknn_toolkit-lite2 (installed)

```

cd rknn_model_zoo/examples/yolov5/python
python3 yolov5.py --model_path /local/demo0_yoloTest/yolov5s_rk3566.rknn --img_save


```