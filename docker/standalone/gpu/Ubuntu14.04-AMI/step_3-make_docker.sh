#!/bin/bash

# To avoid having to use sudo here one must re-login after the previous step
sudo docker build -f ubuntu-nvidia367.57-cuda7.5-cudnn4-gpu-nlpcaffe.docker -t ubuntu-nvidia367.57-cuda7.5-cudnn4-gpu-nlpcaffe:1 .

