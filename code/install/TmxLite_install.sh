#!/bin/bash

# # Install Zlib
# cd ..
# mkdir zlib
# cd zlib
# # Download and install specific version of Zlib (1.2.13)
# wget https://github.com/madler/zlib/archive/refs/tags/v1.2.13.zip
# unzip v1.2.13.zip
# cd zlib-1.2.13
# ./configure
# make -j16
# sudo make install

# # Install TinyXML2
# #sudo apt-get install libtinyxml2-dev -y
# # Remove any existing TinyXML2 version
# #sudo apt-get remove libtinyxml2-dev -y

# cd ../../
# mkdir tinyxml2
# cd tinyxml2
# # Download and install specific version of TinyXML2 (9.0.0)
# wget https://github.com/leethomason/tinyxml2/archive/refs/tags/9.0.0.tar.gz
# tar -xzvf 9.0.0.tar.gz 
# cd tinyxml2-9.0.0
# mkdir build
# cd build
# cmake ..
# make -j 16
# sudo make install

# # Install Zstd
# # Remove any existing Zstd version
# #sudo apt-get remove libzstd-dev -y
# cd ../../..
# mkdir zstd
# cd zstd
# # Download and install specific version of Zstd (1.5.2)
# wget https://github.com/facebook/zstd/archive/refs/tags/v1.5.2.tar.gz 
# tar -xzvf v1.5.2.tar.gz
# cd zstd-1.5.2
# make -j 16
# sudo make install
# cd build/cmake
# cmake .
# make  -j 16
# sudo make install

cd ../external

wget https://github.com/fallahn/tmxlite/archive/refs/heads/master.zip
unzip master.zip
rm master.zip

# cd tmxlite-master && cd tmxlite && mkdir build && cd build
# cmake ..
# sudo make install

