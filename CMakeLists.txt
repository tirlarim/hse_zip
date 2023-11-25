cmake_minimum_required(VERSION 3.21)
project(HSE_zip C)

set(CMAKE_C_STANDARD 23)
#set(CMAKE_C_FLAGS "-Werror -Wall -O3 -march=native")

add_executable(HSE_zip main.c fileWorker/filePrepare.c utils/time.c utils/utils.c
        tree_list/encode.c tree_list/encode.h
        tree_list/decode.c tree_list/decode.h)
