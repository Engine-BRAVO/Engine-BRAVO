#!/bin/bash

# Install Zlib
sudo apt-get install zlib1g-dev -y

# Install TinyXML2
#sudo apt-get install libtinyxml2-dev -y
# Remove any existing TinyXML2 version
#sudo apt-get remove libtinyxml2-dev -y

cd ../../..
# Download and install specific version of TinyXML2 (9.0.0)
wget https://github.com/leethomason/tinyxml2/archive/refs/tags/9.0.0.tar.gz
tar -xzvf 9.0.0.tar.gz 
cd tinyxml2-9.0.0
mkdir build
cd build
cmake ..
make
sudo make install

# Install Zstd
# Remove any existing Zstd version
#sudo apt-get remove libzstd-dev -y

cd ../..
# Download and install specific version of Zstd (1.5.2)
wget https://github.com/facebook/zstd/archive/refs/tags/v1.5.2.tar.gz 
tar -xzvf v1.5.2.tar.gz
cd zstd-1.5.2
make
sudo make install


