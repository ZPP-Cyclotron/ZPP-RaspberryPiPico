add_subdirectory(modbusServer)
add_subdirectory(picoController)
add_subdirectory(nanomodbus)
add_subdirectory(powerSupply)

add_executable(Main Main.cpp)
target_link_libraries(Main modbusServer pico_stdlib)
pico_add_extra_outputs(Main)

add_c_program(blink)
add_cpp_program(irq)
