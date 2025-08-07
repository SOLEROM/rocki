# dev host docker

make build => rock5 ubu22 template dev docker;

## additions

```
sudo apt-get install bzip2 libgl1
wget https://mirrors.tuna.tsinghua.edu.cn/anaconda/miniconda/Miniconda3-4.6.14-Linux-x86_64.sh
chmod 777 Miniconda3-4.6.14-Linux-x86_64.sh
bash Miniconda3-4.6.14-Linux-x86_64.sh


[/home/user/miniconda3] >>> /home/user/shared/condaEnvs

```


```
git clone -b v2.3.0 https://github.com/airockchip/rknn-toolkit2.git
git clone -b v2.3.0 https://github.com/airockchip/rknn_model_zoo.git


conda create -n rknn python=3.8.2

conda activate rknn

# Enter rknn-toolkit2 directory
cd rknn-toolkit2/rknn-toolkit2/packages/x86_64/
pip3 install -r requirements_cp38-2.3.0.txt
pip3 install ./rknn_toolkit2-2.3.0-cp38-cp38-manylinux_2_17_x86_64.manylinux2014_x86_64.whl


```

```
$ python3
>>> from rknn.api import RKNN

```



## add cross compiler

```
pn target get arch => cat /proc/version
Linux version 6.1.84-6-rk2410 (vscode@f6c650d410d5) (aarch64-linux-gnu-gcc 

cd  /opt 
sudo wget https://releases.linaro.org/components/toolchain/binaries/6.3-2017.05/aarch64-linux-gnu/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu.tar.xz

sudo tar xf gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu.tar.xz

export GCC_COMPILER=/opt/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu
```