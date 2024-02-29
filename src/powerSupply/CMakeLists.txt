add_library(powerSupply INTERFACE)

target_include_directories(powerSupply INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if (LINUX_VERSION)
    include_directories(testing)
endif ()