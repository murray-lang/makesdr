cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-updates)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(TRANSPORT_QT_BASIC_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtUpdateClientSideTransport.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtUpdateMessageRings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtUpdateRadioSideTransport.h
)


add_library(core-linux-transport-qt-updates
        ${TRANSPORT_QT_BASIC_FILES}
)

target_include_directories(core-linux-transport-qt-updates PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-linux-transport-qt-updates PUBLIC
        settings-model-message
        core-common-transport-updates
        core-linux-transport-qt-base
        Qt6::Core
)

