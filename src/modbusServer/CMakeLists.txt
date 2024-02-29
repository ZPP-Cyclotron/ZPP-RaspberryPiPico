add_library(modbusServer ModbusServer.cpp)

target_include_directories(modbusServer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(modbusServer picoController powerSupply)
