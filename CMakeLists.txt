cmake_minimum_required(VERSION 3.12)

set(LINUX_VERSION ON)
set(EXAMPLES OFF)

if (NOT LINUX_VERSION)
    include(pico_sdk_import.cmake)
else ()
    if(EXISTS "${CMAKE_BINARY_DIR}/pico-sdk")
        file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/pico-sdk")
    endif()
endif ()

project(ZPP-RaspberryPiPico C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

if (NOT LINUX_VERSION)
    pico_sdk_init()
endif ()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
)

add_subdirectory(src)