cmake_minimum_required(VERSION 4.1)

project(ui-lvgl-radio-display)

set(RADIO_DISPLAY_FILES
        include/ui/lvgl/RadioDisplay.h
        src/RadioDisplay.cpp
)

add_library(ui-lvgl-radio-display STATIC ${RADIO_DISPLAY_FILES})

target_include_directories(ui-lvgl-radio-display PUBLIC
        ${INCLUDE_DIR}
        include
)

target_link_libraries(ui-lvgl-radio-display PUBLIC
        lib-ui
        settings-model-radio-base
        settings-model-radio-iq-common
)