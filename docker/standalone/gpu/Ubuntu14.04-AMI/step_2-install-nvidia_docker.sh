#!/bin/bash

# Output from this command verifies we got our drivers set up
sudo nvidia-smi

# This is mostly a repeat of installs that occurred in step 0 ... here for clarity of what Docker install requires
sudo apt-get update -y
sudo apt-get install -y curl \
    build-essential \
    libsystemd-journal0 \
    linux-image-extra-$(uname -r) \
    linux-image-extra-virtual
    

sudo apt-get install -y apt-transport-https ca-certificates

curl -fsSL https://yum.dockerproject.org/gpg | sudo apt-key add -

wget https://apt.dockerproject.org/repo/pool/main/d/docker-engine/docker-engine_1.10.0-0~trusty_amd64.deb
sudo dpkg -i docker-engine_1.10.0-0~trusty_amd64.deb

sudo groupadd docker
sudo gpasswd -a ${USER} docker
sudo service docker restart


# Install nvidia-docker and nvidia-docker-plugin
wget https://github.com/NVIDIA/nvidia-docker/releases/download/v1.0.0-rc.3/nvidia-docker_1.0.0.rc.3-1_amd64.deb
sudo dpkg -i nvidia-docker_1.0.0.rc.3-1_amd64.deb
