cmake_minimum_required(VERSION 3.21)
project(HSE_zip C)

set(CMAKE_C_STANDARD 23)

add_executable(HSE_zip main.c fileWorker/filePrepare.c fileWorker/filePrepare.h tree_list/tree_list.c tree_list/tree_list.h tree_list/union.h utils/printColors.h)
add_executable(HSE_archive archive.c tree_list/tree_list.h tree_list/union.h)
add_executable(HSE_decode decode.c fileWorker/filePrepare.c fileWorker/filePrepare.h tree_list/tree_list.h tree_list/union.h utils/printColors.h)