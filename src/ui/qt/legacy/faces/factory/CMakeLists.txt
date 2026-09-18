cmake_minimum_required(VERSION 4.1)

project(ui-qt-faces-factory)

set(QT_FACE_FACTORY_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/faces/FaceFactory.h
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/faces/RegisterFace.h

        ${CMAKE_CURRENT_LIST_DIR}/src/FaceFactory.cpp
)

add_library(ui-qt-faces-factory STATIC ${QT_FACE_FACTORY_FILES})

target_include_directories(ui-qt-faces-factory PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_include_directories(ui-qt-faces-factory PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/qt/faces
)

target_link_libraries(ui-qt-faces-factory PUBLIC
        config-struct
        ui-qt-faces-base
)