cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-metering)

#find_package(PkgConfig)
#
#find_package(Qt6 REQUIRED COMPONENTS Core)

set(TRANSPORT_QT_METERING_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtRxMeteringClientSideTransport.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtRxMeteringMessageRings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtRxMeteringRadioSideTransport.h
)


add_library(core-linux-transport-qt-metering
        ${TRANSPORT_QT_METERING_FILES}
)

target_include_directories(core-linux-transport-qt-metering PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-linux-transport-qt-metering PUBLIC
        core-common-transport-metering
        core-linux-transport-qt-base
)

