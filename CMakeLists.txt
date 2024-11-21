cmake_minimum_required(VERSION 3.22.1)

project(server)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 查找源文件
file(GLOB SRC src/*.cpp)
set(SRC ${SRC} main.cpp)

# 创建可执行目标
add_executable(${PROJECT_NAME} ${SRC})

# 设置编译选项
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)

# 设置头文件目录
target_include_directories(${PROJECT_NAME} PRIVATE include)
