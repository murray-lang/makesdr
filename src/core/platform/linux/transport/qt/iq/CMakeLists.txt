cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-iq)

#find_package(PkgConfig)
#
#find_package(Qt6 REQUIRED COMPONENTS Core)

set(TRANSPORT_QT_BASIC_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtIqClientSideTransport.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtIqMessageRings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtIqRadioSideTransport.h
)


add_library(core-linux-transport-qt-iq
        ${TRANSPORT_QT_BASIC_FILES}
)

target_include_directories(core-linux-transport-qt-iq PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-linux-transport-qt-iq PUBLIC
        core-linux-transport-qt-meta
)

