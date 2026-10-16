cmake_minimum_required(VERSION 4.1)

project(example-qt-dual-iq-split-radio VERSION 0.1 LANGUAGES CXX)

#find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Charts)

set (PROJECT_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

add_executable(example-qt-dual-iq-split-radio
        ${PROJECT_SOURCES}
)

target_include_directories(example-qt-dual-iq-split-radio PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(example-qt-dual-iq-split-radio PUBLIC
        config-json
        radios-iq-dual-split
        #        radios-iq-dual-rxtx-settings-model          # For working on this radio type
#        radios-iq-dual-rx-settings-model          # For working on this radio type

        test-utils
        core-linux-audio
        etl::etl
        ui-qt-main-window
#        Qt6::Core
#        Qt6::Gui
#        Qt6::Widgets
        ui-qt-util

#        settings-model-update-util-resolve-many
#        radios-iq-dual-split-settings-update-lookup
#        radios-iq-dual-rxtx-settings-update-lookup  # For working on this radio type
#        radios-iq-dual-rx-settings-update-lookup
)