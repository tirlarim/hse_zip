cmake_minimum_required(VERSION 3.21)
project(HSE_zip C)

set(CMAKE_C_STANDARD 23)
set(build_type "debug")
if (build_type STREQUAL "release")
    set(CMAKE_C_FLAGS "-Werror=vla -Ofast -march=native")
elseif (build_type STREQUAL "debug")
    set(CMAKE_C_FLAGS "-Werror=vla -Og -march=native")
endif ()

add_executable(HSE_zip main.c fileWorker/filePrepare.c utils/time.c utils/utils.c
        tree_list/encode.c tree_list/decode.c)
add_executable(testsAll tests/main.c fileWorker/filePrepare.c utils/time.c utils/utils.c
        tree_list/encode.c tree_list/decode.c)
