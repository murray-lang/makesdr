cmake_minimum_required(VERSION 4.1)

project(ui-common-waterfall)

# Header-only and toolkit-independent: usable from Qt, LVGL, or bare metal
set(WATERFALL_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/common/waterfall/PixelFormat.h
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/common/waterfall/ColorMap.h
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/common/waterfall/Waterfall.h
)

add_library(ui-common-waterfall INTERFACE ${WATERFALL_SOURCES})

target_include_directories(ui-common-waterfall INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(ui-common-waterfall INTERFACE
        settings-model-radio-iq-basic-common
        core-common-dsp-transforms-power-spectrum
)

if(BUILD_TESTING AND IS_LINUX)
    add_subdirectory(tests)
endif()
