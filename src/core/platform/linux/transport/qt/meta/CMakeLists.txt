cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-meta)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(TRANSPORT_QT_BASIC_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtMetaClientSideTransport.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtMetaMessageRings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtMetaRadioSideTransport.h
)


add_library(core-linux-transport-qt-meta
        ${TRANSPORT_QT_BASIC_FILES}
)

target_include_directories(core-linux-transport-qt-meta PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-linux-transport-qt-meta PUBLIC
        settings-model-message
        core-common-transport-meta
        core-linux-transport-qt-base
        Qt6::Core
)

