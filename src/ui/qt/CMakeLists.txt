cmake_minimum_required(VERSION 4.1)

project(ui-qt)

find_package(Qt6 REQUIRED COMPONENTS Core)

# Create shared icons library that can be linked by any Qt target
qt_add_resources(QT_ICON_RESOURCES icons/icons.qrc)
add_library(ui-qt-icons STATIC ${QT_ICON_RESOURCES})
target_link_libraries(ui-qt-icons PUBLIC Qt6::Core)

add_subdirectory(legacy)
add_subdirectory(radio-client)
add_subdirectory(util)

