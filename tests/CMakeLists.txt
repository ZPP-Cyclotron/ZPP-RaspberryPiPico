add_executable(modbusTests modbusTests.cpp)

target_link_libraries(modbusTests GTest::gtest_main linuxController mockPowerSupply)

#include(GoogleTest)
#gtest_discover_tests(modbusTests)