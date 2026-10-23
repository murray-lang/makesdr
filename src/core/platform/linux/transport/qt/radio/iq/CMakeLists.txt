cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-radio-iq)

add_library(core-linux-transport-qt-radio-iq INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtIqCapableRadioTransportT.h
)

target_include_directories(core-linux-transport-qt-radio-iq INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(core-linux-transport-qt-radio-iq INTERFACE
        core-common-transport-radio
        core-linux-transport-qt-settings
        core-linux-transport-qt-iq
        core-linux-transport-qt-meta
        core-linux-transport-qt-metering
        core-linux-transport-qt-spectrum
        core-linux-transport-qt-updates
)