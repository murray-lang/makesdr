cmake_minimum_required(VERSION 4.1)

project(ui)

add_subdirectory(common)

if(IS_QT)
    add_subdirectory(qt)
endif()
