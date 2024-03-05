add_executable(modbusTests modbusTests.cpp)

target_link_libraries(modbusTests GTest::gtest_main modbusServer mockPicoController mockPowerSupply)

#include(GoogleTest)
#gtest_discover_tests(modbusTests)