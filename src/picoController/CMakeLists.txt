#add_library(nanomodbus nanomodbus.c nanomodbus.h)
#target_link_libraries(nanomodbus pico_stdlib)
#
#add_executable(ModbusServer nanomodbus.c
#        ../Main.cpp
#        ModbusServer.cpp
#        ModbusServer.hpp
#        ../pico/PicoController.hpp
#        ../pico/PicoControllerInterface.hpp
#        ../pico/PicoController.cpp
#        ../powerSupply/PowerSupplyInterface.hpp
#        ../powerSupply/testing/MockPowerSupply.hpp
#        ../pico/testing/MockPicoController.hpp
#        ../../tests/modbusTests.cpp
#)
#target_link_libraries(ModbusServer nanomodbus pico_stdlib)
#pico_add_extra_outputs(ModbusServer)


add_library(picoController PicoController.cpp)

target_include_directories(picoController PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(picoController pico_stdlib nanomodbus)
