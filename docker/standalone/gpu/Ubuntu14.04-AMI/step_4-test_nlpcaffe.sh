#!/bin/bash
sudo docker run -it --device=/dev/nvidiactl --device=/dev/nvidia-uvm --device=/dev/nvidia0 ubuntu-nvidia367.57-cuda7.5-cudnn4-gpu-nlpcaffe:1  /bin/bash -c 'cd /home/caffe-user/nlpcaffe ; ./data/language_model/get_lm.sh; python ./examples/language_model/create_lm.py --make_data ; ./examples/language_model/train_lm.sh'
