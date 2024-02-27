if (NOT LINUX_VERSION)
    add_library(picoController PicoController.cpp)

    target_include_directories(picoController PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(picoController pico_stdlib nanomodbus)
else ()

    add_library(picoController INTERFACE)

    target_include_directories(picoController INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

    add_library(linuxController LinuxController.cpp)

    target_include_directories(linuxController PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


    target_link_libraries(linuxController nanomodbus)
endif ()