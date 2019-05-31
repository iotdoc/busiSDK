# paas_aksk_sdk

提供client sdk，用户可在控制台下载。下载后直接使用

需要安装
libssl.so
libcrypto.so
libz.so

下载到目录下
g++ -o sample sample.cpp -I./include -L./lib -lauth

export LD_LIBRARY_PATH=./lib/

./sample

