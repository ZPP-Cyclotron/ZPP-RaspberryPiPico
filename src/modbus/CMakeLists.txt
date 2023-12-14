add_library(nanomodbus nanomodbus.c nanomodbus.h)
target_link_libraries(nanomodbus pico_stdlib)

add_executable(modbus_example modbus_example.cpp nanomodbus.c)
target_link_libraries(modbus_example nanomodbus pico_stdlib)
pico_add_extra_outputs(modbus_example)
