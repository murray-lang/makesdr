cmake_minimum_required(VERSION 4.1)

project(ui-qt-faces-standard)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(STANDARD_FACE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/faces/standard/StandardFace.h
        ${CMAKE_CURRENT_LIST_DIR}/src/StandardFace.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/StandardFace.ui
)

# Create internal OBJECT library to force linking of REGISTER_FACE() initializer
add_library(ui-qt-faces-standard-obj OBJECT ${STANDARD_FACE_FILES})

target_include_directories(ui-qt-faces-standard-obj PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_include_directories(ui-qt-faces-standard-obj PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/faces/standard
)

target_link_libraries(ui-qt-faces-standard-obj PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        ui-qt-faces-base
        ui-qt-faces-factory
        settings-model-radios-iq-dual-split
        ui-qt-widgets-frequency-panel
        ui-qt-widgets-charts-timeseries
        ui-qt-widgets-charts-panadapter
        ui-qt-widgets-s-meter
)

# INTERFACE library that exposes the OBJECT library to consumers
# This ensures the OBJECT library is always linked (not dropped by linker)
add_library(ui-qt-faces-standard INTERFACE)

target_sources(ui-qt-faces-standard INTERFACE $<TARGET_OBJECTS:ui-qt-faces-standard-obj>)

target_include_directories(ui-qt-faces-standard INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(ui-qt-faces-standard INTERFACE
        ui-qt-faces-standard-obj
)