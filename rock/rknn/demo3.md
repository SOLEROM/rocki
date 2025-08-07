# run Ultralytics

Currently, the Ultralytics library officially supports the RKNN platform. Users of RK3588/356X products can directly use the ultralytics library for YOLOv11 model conversion and deployment.

```
source ~/proj/zooVenv/bin/activate
pip3 install -U ultralytics

cd /home/rock/proj/ultra
wget https://github.com/zifeng-radxa/rknn_model_zoo/releases/download/yolov11/yolo11n_3588_rknn_model.zip
unzip yolo11n_3588_rknn_model.zip 

yolo predict model='yolo11n_3588_rknn_model' source=./test.jpg

```

* camera stream use source=X depends on ( v4l2-ctl --list-devices )
