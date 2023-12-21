# 'Google_test' is the subproject name
project(Google_tests CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Werror=vla -march=native")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
find_package(PkgConfig REQUIRED)
find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIRS})
    include_directories(/usr/local/opt/openssl/include)
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
else()
    message(FATAL_ERROR "Unable to find OpenSSL")
endif()

# 'Google_Tests_run' is the target name
# 'test1.cpp test2.cpp' are source files with tests
add_executable(Google_Tests_run test-text.cpp test-bin.cpp test-heavy.cpp main.cpp utils/test-utils.cpp ../tree_list/decode.c ../tree_list/encode.c
        ../fileWorker/filePrepare.c ../utils/time.c ../utils/utils.c)
target_include_directories(Google_Tests_run PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(Google_Tests_run PRIVATE gtest gtest_main OpenSSL::SSL OpenSSL::Crypto)


