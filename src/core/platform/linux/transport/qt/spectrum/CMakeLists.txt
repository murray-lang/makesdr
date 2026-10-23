cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-spectrum)

#find_package(PkgConfig)
#
#find_package(Qt6 REQUIRED COMPONENTS Core)

set(TRANSPORT_QT_SPECTRUM_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtSpectrumClientSideTransport.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtSpectrumMessageRings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/QtSpectrumRadioSideTransport.h
)


add_library(core-linux-transport-qt-spectrum
        ${TRANSPORT_QT_SPECTRUM_FILES}
)

target_include_directories(core-linux-transport-qt-spectrum PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-linux-transport-qt-spectrum PUBLIC
        core-common-transport-spectrum
        core-linux-transport-qt-base
)

