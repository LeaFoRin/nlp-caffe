#!/bin/bash

# START WITH AMI (or regional equivalent):
#  ubuntu/images/hvm-ssd/ubuntu-trusty-14.04-amd64-server-20161213 (ami-5ac2cd4d)
#  launch on p2.xlarge instance

# Reference:
#   https://github.com/NVIDIA/nvidia-docker/wiki/Deploy-on-Amazon-EC2

sudo apt-get -q update -y
sudo apt-get -q install -y build-essential kernel-package linux-image-extra-`uname -r` linux-image-extra-virtual wget 
sudo apt-get -q update -y

CURRRENT_VER_LINE=$(cat cat /proc/driver/nvidia/version | head -1 )
if [[ $CURRRENT_VER_LINE == *"367.57"* ]]; then
  echo "We have the correct version"
  echo "Skip the reboot and continue to step 2"
  exit
fi

# Install NVIDIA drivers 367.57
#  This is incrementally newer than the version 367.48 that the Ubuntu repos hold.
#  NVidia no longer has the 367.48 version on their downloads site and we have
#  to have the *.run file so that we match the host and the Docker container
wget http://us.download.nvidia.com/XFree86/Linux-x86_64/367.57/NVIDIA-Linux-x86_64-367.57.run
sudo sh NVIDIA-Linux-x86_64-367.57.run --ui=none --no-questions --accept-license
