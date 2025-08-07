# onnx directly run


deps:
	sudo apt install -y python3-pip python3-dev libprotobuf-dev protobuf-compiler


# runtime CPU
	python3 -m venv onnxCPU
	pip3 install onnx onnxruntime
	pip3 install torch
	pip3 install torchaudio
	sudo apt install sox libsox-dev libsox-fmt-all libsndfile1
	
	
