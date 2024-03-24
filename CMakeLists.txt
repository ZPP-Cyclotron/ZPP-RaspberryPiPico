cmake_minimum_required(VERSION 3.12)

set(MODBUS_TIMEOUT 500000) # microseconds
set(BAUD_RATE 38400) # Hz, should be 38400
set(UART_TX_PIN 0)
set(UART_RX_PIN 1)

set(LINUX_VERSION ON)

# Only for testing:
set(VIRTUAL_PORT_LINUX "/dev/pts/3")
set(ILLEGAL_DATA_VALUE_LINUX_TEST 0)
set(ANSWERS_ONE_OF_N_REQUESTS_LINUX_TEST 0)
set(ONE_OF_N_REQUESTS_IS_RANDOM_BYTES_LINUX_TEST 0)
set(ERROR_RESPONSE_TO_ONE_OF_N_READ_REQUESTS_LINUX_TEST 0)

configure_file (config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h @ONLY)
set(LINUX_VERSION OFF)
set(EXAMPLES OFF)
set(200A ON)

if (NOT LINUX_VERSION)
    include(pico_sdk_import.cmake)
else ()
    include(google_test_import.cmake)
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

if (LINUX_VERSION)
    add_subdirectory(tests)
endif ()
