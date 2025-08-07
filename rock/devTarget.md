# RKNN-Toolkit2 


## target : update and enable NPU

* deps:

```
sudo rsetup -> system -> System Update

sudo rsetup -> Overlays -> Manage overlays -> Enable NPU ((For users of RK356X products))
```


## option1: use on target 

```
sudo apt update
sudo apt install python3-rknnlite2
sudo apt install rknpu2-rk3588

```



## option2: use venv with wheel

```
sudo apt install python3.11-venv
python3 -m venv zooVenv
source ~/proj/zooVenv/bin/activate

### in vnev:
pip3 install --upgrade pip

python --version
Python 3.11.2


```

* copy from host dev docker to venv

```
scp ~/shared/rknn-toolkit2/rknn-toolkit-lite2/packages/rknn_toolkit_lite2-2.3.0-cp311-cp311-manylinux_2_17_aarch64.manylinux2014_aarch64.whl rock@<x.y.z.u>/tmp

pip3 install  /tmp/rknn_toolkit_lite2-2.3.0-cp311-cp311-manylinux_2_17_aarch64.manylinux2014_aarch64.whl 

pip3 install opencv-python-headless

```


## target gits

```
mkdir ~/proj && cd ~/proj

# Download RKNN Model Zoo repository
git clone -b v2.3.0 https://github.com/airockchip/rknn_model_zoo.git


```



## test

```
sudo dmesg | grep "Initialized rknpu"

$ python3
>>> from rknnlite.api import RKNNLite as RKNN

```
