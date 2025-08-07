# yolo demo

Deploying YOLOv5 with RKNN involves two steps:

* Use rknn-toolkit2 on a PC to convert models from various frameworks to RKNN format.
* Use the Python API of rknn-toolkit2-lite on the device for model inference.


### host side

```
conda activate rknn
cd /home/user/shared/rknn_model_zoo/examples/yolov5/model

bash download_model.sh
        Saving to: './yolov5s_relu.onnx'


cd ../python
python3 convert.py ../model/yolov5s_relu.onnx  yolov5s_relu_rk3588.rknn

scp rknn to target

```

## target


```
rewrite:
    rknn_model_zoo/py_utils/rknn_executor.py
    =========================================

from rknnlite.api import RKNNLite as RKNN

class RKNN_model_container():
    def __init__(self, model_path, target=None, device_id=None) -> None:
        rknn = RKNN()
        rknn.load_rknn(model_path)
        ret = rknn.init_runtime()
        self.rknn = rknn

    def run(self, inputs):
        if not self.rknn:
            print("ERROR: RKNN has been released")
            return []

        inputs = [inputs] if not isinstance(inputs, (list, tuple)) else inputs
        result = self.rknn.inference(inputs=inputs)
        return result

    def release(self):
        self.rknn.release()
        self.rknn = None


```

### target

```
pip install opencv-python --break-system-packages

cd /home/rock/proj/rknn_model_zoo/examples/yolov5/python

python3 yolov5.py --model_path ../model/yolov5s_relu_rk3588.rknn --img_save

```

### venv run

```
(zooVenv) rock@rock-5b:~/proj/rknn_model_zoo/examples/yolov5/python$ python3 yolov5.py --model_path ../model/yolov5s_relu_rk3588.rknn --img_save
use anchors from '../model/anchors_yolov5.txt', which is [[[10.0, 13.0], [16.0, 30.0], [33.0, 23.0]], [[30.0, 61.0], [62.0, 45.0], [59.0, 119.0]], [[116.0, 90.0], [156.0, 198.0], [373.0, 326.0]]]
W rknn-toolkit-lite2 version: 2.3.0
I RKNN: [11:42:26.729] RKNN Runtime Information, librknnrt version: 2.3.0 (c949ad889d@2024-11-07T11:35:33)
I RKNN: [11:42:26.731] RKNN Driver Information, version: 0.9.8
I RKNN: [11:42:26.737] RKNN Model Information, version: 6, toolkit version: 2.3.0(compiler version: 2.3.0 (c949ad889d@2024-11-07T11:39:30)), target: RKNPU v2, target platform: rk3588, framework name: ONNX, framework layout: NCHW, model inference type: static_shape
W RKNN: [11:42:26.795] query RKNN_QUERY_INPUT_DYNAMIC_RANGE error, rknn model is static shape type, please export rknn with dynamic_shapes
W Query dynamic range failed. Ret code: RKNN_ERR_MODEL_INVALID. (If it is a static shape RKNN model, please ignore the above warning message.)
Model-../model/yolov5s_relu_rk3588.rknn is rknn model, starting val
infer 1/1

IMG: bus.jpg
person @ (209 243 286 510) 0.880
person @ (479 238 560 526) 0.871
person @ (109 238 231 534) 0.840
person @ (79 353 121 517) 0.301
bus  @ (91 129 555 464) 0.692
Detection result save to ./result/bus.jpg

```