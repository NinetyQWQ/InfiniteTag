cmake_minimum_required(VERSION 3.22.1)

project("InfiniteTag")

add_terraria_mod(InfiniteTag
        mod.cpp
)

target_include_directories(InfiniteTag PUBLIC ../TEFKernel-Cpp-Wrapper-main)