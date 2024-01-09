add_library(nanomodbus nanomodbus.c nanomodbus.h)
target_link_libraries(nanomodbus pico_stdlib)

add_executable(ModbusServer ModbusServer.cpp nanomodbus.c)
target_link_libraries(ModbusServer nanomodbus pico_stdlib)
pico_add_extra_outputs(ModbusServer)
