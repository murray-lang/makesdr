cmake_minimum_required(VERSION 4.1)

project(radios-iq-dual-rx-transport-qt-globals)

set(QT_TRANSPORT_GLOBALS_IQ_DUAL_RX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/globalQtDualIqRxRings.h
        ${CMAKE_CURRENT_LIST_DIR}/src/globalQtDualIqRxRings.cpp
)

add_library(radios-iq-dual-rx-transport-qt-globals STATIC ${QT_TRANSPORT_GLOBALS_IQ_DUAL_RX_FILES})

target_include_directories(radios-iq-dual-rx-transport-qt-globals PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(radios-iq-dual-rx-transport-qt-globals PUBLIC
        core-linux-transport-qt-settings
        settings-model-radio-iq-dual-rx
)