add_subdirectory(modbusServer)
add_subdirectory(picoController)
add_subdirectory(nanomodbus)
add_subdirectory(powerSupply)

if (NOT LINUX_VERSION)

    if (POWER_SUPPLY EQUAL 200)
        add_compile_definitions( PS200A )

    elseif (POWER_SUPPLY EQUAL 100)
        #    ...

    endif ()

    add_executable(Main Main.cpp)
    target_link_libraries(Main modbusServer powerSupply pico_stdlib)
   
    if (POWER_SUPPLY EQUAL 200)
        # enable usb output, disable uart output
        pico_enable_stdio_usb(Main 1)
        pico_enable_stdio_uart(Main 0)
    endif ()
    
    pico_add_extra_outputs(Main)

else ()

    add_executable(LinuxMain LinuxMain.cpp)
    target_link_libraries(LinuxMain modbusServer linuxController)

endif ()

