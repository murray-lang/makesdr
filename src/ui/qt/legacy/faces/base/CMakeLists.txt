cmake_minimum_required(VERSION 4.1)

project(ui-qt-faces-base)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(FACE_BASE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/faces/FaceBase.h
        ${CMAKE_CURRENT_LIST_DIR}/src/FaceBase.cpp
)

add_library(ui-qt-faces-base STATIC ${FACE_BASE_FILES})

target_include_directories(ui-qt-faces-base PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(ui-qt-faces-base PUBLIC
        Qt6::Widgets
        core-common-samples
        settings-model-radios-iq-dual-split
)