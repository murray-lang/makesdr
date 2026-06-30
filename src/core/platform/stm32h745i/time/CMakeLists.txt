cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-time)


if(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    add_library(core-stm32h745i-time-cm4 STATIC ${CMAKE_CURRENT_LIST_DIR}/src/Timestamp.cpp)

    target_link_libraries(core-stm32h745i-time-cm4 PUBLIC
            core-common-time
            core-stm32h745i-drivers-hal-cm4
    )
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    add_library(core-stm32h745i-time-cm7 STATIC ${CMAKE_CURRENT_LIST_DIR}/src/Timestamp.cpp)

    target_link_libraries(core-stm32h745i-time-cm7 PUBLIC
            core-common-time
            core-stm32h745i-drivers-hal-cm7
    )
endif()

