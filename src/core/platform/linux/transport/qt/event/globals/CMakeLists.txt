cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-globals)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(TRANSPORT_QT_EVENT_TARGET_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/event/globalQtEventTargets.h
        ${CMAKE_CURRENT_LIST_DIR}/src/globalQtEventTargets.cpp
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/qt/event/globalQtMessageExchange.h
        ${CMAKE_CURRENT_LIST_DIR}/src/globalQtMessageExchange.cpp
)

add_library(core-linux-transport-qt-globals STATIC ${TRANSPORT_QT_EVENT_TARGET_FILES})

target_include_directories(core-linux-transport-qt-globals PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-linux-transport-qt-globals PUBLIC
        config-struct
        core-common-transport-radio
        core-linux-transport-qt-event-base
        settings-model-radio-selected
        Qt6::Core
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()