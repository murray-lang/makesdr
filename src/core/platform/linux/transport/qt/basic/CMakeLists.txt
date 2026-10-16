cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-basic)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(TRANSPORT_QT_BASIC_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtBasicClientSideTransport.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtBasicMessageRings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtBasicRadioSideTransport.h
)


add_library(core-linux-transport-qt-basic
        ${TRANSPORT_QT_BASIC_FILES}
)

target_include_directories(core-linux-transport-qt-basic PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-linux-transport-qt-basic PUBLIC
        settings-model-message
        core-common-transport-basic
        core-linux-transport-qt-base
        Qt6::Core
)

