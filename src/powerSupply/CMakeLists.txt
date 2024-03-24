if (NOT LINUX_VERSION)
    add_library(powerSupply PowerSupply200A.hpp)

    target_include_directories(powerSupply PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(powerSupply pico_stdlib hardware_spi)

else ()
    add_library(powerSupply INTERFACE)

    target_include_directories(powerSupply INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
    include_directories(testing)
endif ()