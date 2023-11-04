cmake_minimum_required(VERSION 3.21)
project(HSE_zip C)

set(CMAKE_C_STANDARD 23)

add_executable(HSE_zip main.c fileWorker/filePrepare.c tree_list/tree_list.c utils/time.c utils/utils.c)
#add_executable(HSE_archive archive.c tree_list/tree_list.h tree_list/union.h utils/time.c utils/time.h utils/utils.c utils/utils.h)
#add_executable(HSE_decode decode.c fileWorker/filePrepare.c fileWorker/filePrepare.h tree_list/tree_list.h tree_list/union.h utils/printColors.h utils/time.c utils/time.h utils/utils.c utils/utils.h)