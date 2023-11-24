cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(ZPP-RaspberryPiPico C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
)

function(add_cpp_program target_name)
    add_executable(${target_name} "${target_name}.cpp")
    target_link_libraries(${target_name} pico_stdlib)
    pico_add_extra_outputs(${target_name})
endfunction()

function(add_c_program target_name)
    add_executable(${target_name} "${target_name}.c")
    target_link_libraries(${target_name} pico_stdlib)
    pico_add_extra_outputs(${target_name})
endfunction()

add_subdirectory(src)