add_library(mockPowerSupply INTERFACE MockPowerSupply.hpp)

target_include_directories(mockPowerSupply INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mockPowerSupply INTERFACE gtest gmock)
