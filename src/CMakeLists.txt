add_subdirectory(modbusServer)
add_subdirectory(picoController)
add_subdirectory(nanomodbus)
add_subdirectory(powerSupply)

if (NOT LINUX_VERSION)
    add_executable(Main Main.cpp)
    target_link_libraries(Main modbusServer pico_stdlib)
    pico_add_extra_outputs(Main)
endif ()

if (EXAMPLES)
    add_executable(irq irq.cpp)
    target_link_libraries(irq pico_stdlib)
    pico_add_extra_outputs(irq)

    add_executable(blink blink.c)
    target_link_libraries(blink pico_stdlib)
    pico_add_extra_outputs(blink)
endif ()

if (LINUX_VERSION)
    add_executable(LinuxMain LinuxMain.cpp)
    target_link_libraries(LinuxMain modbusServer linuxController)
endif ()

