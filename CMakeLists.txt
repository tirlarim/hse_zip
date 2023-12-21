cmake_minimum_required(VERSION 3.21)
project(HSE_zip C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "-Werror=vla -march=native")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Werror=vla -march=native")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_subdirectory(Google_tests)

add_executable(HSE_zip main.c fileWorker/filePrepare.c utils/time.c utils/utils.c
        tree_list/encode.c tree_list/decode.c)
