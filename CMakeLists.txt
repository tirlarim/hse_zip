cmake_minimum_required(VERSION 3.21)
project(HSE_zip C)

set(CMAKE_C_STANDARD 23)

add_executable(HSE_zip main.c fileWorker/filePrepare.c fileWorker/filePrepare.h tree_list/tree_list.c tree_list/tree_list.h tree_list/union.h)
