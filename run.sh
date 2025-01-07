#!/bin/bash

# 切换到 build 目录
cd build/ || { echo "Failed to change directory to 'build/'"; exit 1; }

cmake ..

# 执行 make
make || { echo "Make failed"; exit 1; }

# 返回上一级目录
cd ..

# 启动服务

python3 ./HTTPServer/HTTPServer.py &

./server
echo "Server started successfully"
