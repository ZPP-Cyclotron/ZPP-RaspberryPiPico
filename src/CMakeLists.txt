add_subdirectory(modbusServer)
add_subdirectory(picoController)
add_subdirectory(nanomodbus)
add_subdirectory(powerSupply)

if (NOT LINUX_VERSION)
    add_executable(Main Main.cpp)
    target_link_libraries(Main modbusServer pico_stdlib)
    pico_add_extra_outputs(Main)
endif ()

if (LINUX_VERSION)
    add_executable(LinuxMain LinuxMain.cpp)
    target_link_libraries(LinuxMain modbusServer linuxController)
endif ()

