cmake_minimum_required(VERSION 4.1)

project(ui-qt-radio-client)



find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(ui-qt-radio-client STATIC
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/QtRadioClient.h
#        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/QtRadioClientBase.h
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/QtRadioClientT.h
        ${CMAKE_CURRENT_LIST_DIR}/src/QtRadioClient.cpp
)

# Enable automatic MOC processing
set_target_properties(ui-qt-radio-client PROPERTIES
        AUTOMOC ON
)

target_include_directories(ui-qt-radio-client PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(ui-qt-radio-client PUBLIC
        Qt6::Core
#        linux-test-utils
        radios-base
        radios-settings
        core-common-samples
        settings-model-meta-radio
#        settings-model-radio-selected
#        core-common-transport-radio
#        core-linux-transport-qt-event-base
#        core-linux-transport-qt-globals
        core-common-transport-settings
        core-common-transport-updates
        core-common-transport-meta
        core-common-transport-iq
        radios-iq-dual-split-transport-qt-globals
        radios-iq-dual-rxtx-transport-qt-globals
        radios-iq-dual-rx-transport-qt-globals
#        radios-iq-single-rxtx-transport-qt-globals
        radios-iq-single-rx-transport-qt-globals
        core-linux-transport-qt-globals-meta
        core-linux-transport-qt-globals-iq
        core-linux-transport-qt-globals-metering
        core-linux-transport-qt-globals-spectrum
        core-linux-transport-qt-globals-updates
        settings-model-radio-iq-dual-split
        settings-model-radio-iq-dual-rxtx
        settings-model-radio-iq-dual-rx
        settings-model-radio-iq-single-rxtx
        settings-model-radio-iq-single-rx


#        core-linux-event-data
#        core-linux-globals
#        settings-control-qt-globals
#        settings-model-update-radios-update-helpers
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
