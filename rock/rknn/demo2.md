# run onnx

* run onnx from model zoo

```
source ~/proj/zooVenv/bin/activate
pip install onnxruntime

(zooVenv) rock@rock-5b:~/proj/rknn_model_zoo/examples/yolov5/python$ python3 yolov5.py --model_path ../model/yolov5s_relu.onnx --img_save
use anchors from '../model/anchors_yolov5.txt', which is [[[10.0, 13.0], [16.0, 30.0], [33.0, 23.0]], [[30.0, 61.0], [62.0, 45.0], [59.0, 119.0]], [[116.0, 90.0], [156.0, 198.0], [373.0, 326.0]]]
Model-../model/yolov5s_relu.onnx is onnx model, starting val
WARNING: reshape inputdata-0: from (1, 3, 640, 640) to [1, 3, 640, 640]


IMG: bus.jpg
person @ (208 242 286 508) 0.881
person @ (478 238 560 525) 0.859
person @ (109 237 232 534) 0.842
person @ (79 355 121 515) 0.318
bus  @ (91 129 555 465) 0.702
Detection result save to ./result/bus.jpg


```