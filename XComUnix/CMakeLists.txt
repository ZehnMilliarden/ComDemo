cmake_minimum_required(VERSION 3.0)

# 设置版本
project(XComDemo VERSION 1.0.0.0 LANGUAGES C CXX)
## project 指令同时会影响到 PROJECT_SOURCE_DIR 变量的值

# 指定 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src/XComClient)
add_subdirectory(src/XComServer)