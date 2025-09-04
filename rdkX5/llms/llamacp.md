# hobot_llamacp

* https://github.com/D-Robotics/hobot_llamacpp

### install

```
source /opt/tros/humble/setup.bash
mkdir ~/ros2_ws/src
git clone https://github.com/D-Robotics/hobot_llamacpp


# pull llama.cpp project
git clone https://github.com/ggml-org/llama.cpp -b b4749
cd llama.cpp/
# build third party
cmake -B build
cmake --build build --config Release
```

```
tree need to be:

    cd hobot_llamacpp && ln -s ../llama.cpp llama.cpp

    root@ubuntu:~/ros2_ws/src# tree -L 2
    ├── hobot_llamacpp
    │   └── llama.cpp -> ../llama.cpp
    └── llama.cpp
```


```
sudo apt-get update
sudo apt-get install -y python3-colcon-common-extensions python3-rosdep 


colcon build --merge-install --cmake-args -DPLATFORM_X5=ON --packages-select hobot_llamacpp

Starting >>> hobot_llamacpp
[23.0s] [0/1 complete] [hobot_llamacpp:build 15% - 21.0s]
...
...........




```

### run


```
cd ~/ros2_ws
mkdir -p models

# 1) Vision encoder (X5)
wget -O models/vit_model_int16_v2.bin \
  https://huggingface.co/D-Robotics/InternVL2_5-1B-GGUF-BPU/resolve/main/rdkx5/vit_model_int16_v2.bin

# 2) LLM
wget -O models/Qwen2.5-0.5B-Instruct-Q4_0.gguf \
  https://huggingface.co/D-Robotics/InternVL2_5-1B-GGUF-BPU/resolve/main/Qwen2.5-0.5B-Instruct-Q4_0.gguf


///smaller?

# SigLIP encoder for X5 (≈186 MB)
wget -O SigLip_int16_SmolVLM2_256M_Instruct_MLP_C1_UP_X5.bin \
  "https://huggingface.co/D-Robotics/SmolVLM2-256M-Video-Instruct-GGUF-BPU/resolve/main/rdkx5/SigLip_int16_SmolVLM2_256M_Instruct_MLP_C1_UP_X5.bin"

# SmolVLM2 LLM (Q8_0, ≈175 MB)
wget -O SmolVLM2-256M-Video-Instruct-Q8_0.gguf \
  "https://huggingface.co/D-Robotics/SmolVLM2-256M-Video-Instruct-GGUF-BPU/resolve/main/SmolVLM2-256M-Video-Instruct-Q8_0.gguf"

```


```
cd ~/ros2_ws
source /opt/tros/humble/setup.bash
source ~/ros2_ws/install/setup.bash
export COLCON_CURRENT_PREFIX=./install
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/ros2_ws/install/lib
cp -r install/lib/hobot_llamacpp/config/ . 




ros2 run hobot_llamacpp hobot_llamacpp   --ros-args   -p feed_type:=0   -p model_type:=1   -p image:=config/image1.jpg -p image_type:=0   -p user_prompt:="Describe the image in one sentence."   -p model_file_name:=/root/ros2_ws/models/SigLip_int16_SmolVLM2_256M_Instruct_MLP_C1_UP_X5.bin   -p llm_model_name:=/root/ros2_ws/models/SmolVLM2-256M-Video-Instruct-Q8_0.gguf



```


## git models?

```
find . -name "*.gguf*"
./src/llama.cpp/models/ggml-vocab-deepseek-coder.gguf.out
./src/llama.cpp/models/ggml-vocab-llama-spm.gguf.inp
./src/llama.cpp/models/ggml-vocab-roberta-bpe.gguf.inp
./src/llama.cpp/models/ggml-vocab-llama-bpe.gguf.inp
./src/llama.cpp/models/ggml-vocab-aquila.gguf
./src/llama.cpp/models/ggml-vocab-phi-3.gguf.out
./src/llama.cpp/models/ggml-vocab-chameleon.gguf.inp
./src/llama.cpp/models/ggml-vocab-mpt.gguf.out
./src/llama.cpp/models/ggml-vocab-chameleon.gguf.out
./src/llama.cpp/models/ggml-vocab-deepseek-r1-qwen.gguf.out
./src/llama.cpp/models/ggml-vocab-falcon.gguf
./src/llama.cpp/models/ggml-vocab-deepseek-r1-qwen.gguf.inp
./src/llama.cpp/models/ggml-vocab-bert-bge.gguf.inp
./src/llama.cpp/models/ggml-vocab-roberta-bpe.gguf.out
./src/llama.cpp/models/ggml-vocab-deepseek-coder.gguf.inp
./src/llama.cpp/models/ggml-vocab-starcoder.gguf
./src/llama.cpp/models/ggml-vocab-refact.gguf
./src/llama.cpp/models/ggml-vocab-falcon.gguf.inp
./src/llama.cpp/models/ggml-vocab-deepseek-llm.gguf.out
./src/llama.cpp/models/ggml-vocab-refact.gguf.out
./src/llama.cpp/models/ggml-vocab-llama-spm.gguf
./src/llama.cpp/models/ggml-vocab-mpt.gguf.inp
./src/llama.cpp/models/ggml-vocab-phi-3.gguf
./src/llama.cpp/models/ggml-vocab-llama-bpe.gguf.out
./src/llama.cpp/models/ggml-vocab-qwen2.gguf
./src/llama.cpp/models/ggml-vocab-gpt-2.gguf.inp
./src/llama.cpp/models/ggml-vocab-starcoder.gguf.inp
./src/llama.cpp/models/ggml-vocab-bert-bge.gguf.out
./src/llama.cpp/models/ggml-vocab-deepseek-llm.gguf
./src/llama.cpp/models/ggml-vocab-refact.gguf.inp
./src/llama.cpp/models/ggml-vocab-starcoder.gguf.out
./src/llama.cpp/models/ggml-vocab-qwen2.gguf.inp
./src/llama.cpp/models/ggml-vocab-bert-bge.gguf
./src/llama.cpp/models/ggml-vocab-command-r.gguf
./src/llama.cpp/models/ggml-vocab-deepseek-llm.gguf.inp
./src/llama.cpp/models/ggml-vocab-qwen2.gguf.out
./src/llama.cpp/models/ggml-vocab-baichuan.gguf
./src/llama.cpp/models/ggml-vocab-falcon.gguf.out
./src/llama.cpp/models/ggml-vocab-llama-spm.gguf.out
./src/llama.cpp/models/ggml-vocab-command-r.gguf.out
./src/llama.cpp/models/ggml-vocab-gpt-2.gguf
./src/llama.cpp/models/ggml-vocab-command-r.gguf.inp
./src/llama.cpp/models/ggml-vocab-phi-3.gguf.inp
./src/llama.cpp/models/ggml-vocab-llama-bpe.gguf
./src/llama.cpp/models/ggml-vocab-mpt.gguf
./src/llama.cpp/models/ggml-vocab-gpt-neox.gguf
./src/llama.cpp/models/ggml-vocab-gpt-2.gguf.out
./src/llama.cpp/models/ggml-vocab-deepseek-coder.gguf
```