cmake_minimum_required(VERSION 4.1)

project(ui-qt-widgets-scale-renderer VERSION 0.1 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set (QT_PROPERTY_SETTER_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets/LinearTickProvider.h
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets/NiceNumberTickProvider.h
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets/ScaleLayoutHelpers.h
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets/ScaleModelHelpers.h
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets/ScaleRenderer.h

        ${CMAKE_CURRENT_LIST_DIR}/src/LinearTickProvider.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/NiceNumberTickProvider.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/ScaleRenderer.cpp
)

add_library(ui-qt-widgets-scale-renderer STATIC ${QT_PROPERTY_SETTER_SOURCES})

target_include_directories(ui-qt-widgets-scale-renderer PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_include_directories(ui-qt-widgets-scale-renderer PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/widgets
)

target_link_libraries(ui-qt-widgets-scale-renderer PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)