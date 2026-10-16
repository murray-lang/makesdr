cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-settings)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(TRANSPORT_QT_BASIC_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtSettingsClientSideTransportT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtSettingsMessageRingsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtSettingsRadioSideTransportT.h
)


add_library(core-linux-transport-qt-settings
        ${TRANSPORT_QT_BASIC_FILES}
)

target_include_directories(core-linux-transport-qt-settings PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-linux-transport-qt-settings PUBLIC
        settings-model-message
        core-common-transport-settings
        core-linux-transport-qt-base
        Qt6::Core
)

