add_subdirectory(modbusServer)
add_subdirectory(picoController)
add_subdirectory(nanomodbus)
add_subdirectory(powerSupply)

if (NOT LINUX_VERSION)
    add_executable(Main Main.cpp)
    target_link_libraries(Main modbusServer powerSupply pico_stdlib)
    pico_add_extra_outputs(Main)
endif ()

if (200A)
    add_executable(testPS testPS.cpp)
    target_link_libraries(testPS powerSupply pico_stdlib)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(testPS 1)
    pico_enable_stdio_uart(testPS 0)

    pico_add_extra_outputs(testPS)
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

