cmake_minimum_required(VERSION 4.1)

project(core-linux-transport-qt-event-in-out)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(QT_TRANSPORT_IN_OUT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/in-out/QtObjectBase.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/in-out/QtTransportInOutT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/in-out/QtTransportIoConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/src/QtObjectBase.cpp
)

add_library(core-linux-transport-qt-event-in-out STATIC ${QT_TRANSPORT_IN_OUT_FILES})

set_target_properties(core-linux-transport-qt-event-in-out PROPERTIES
        AUTOMOC ON
)

target_include_directories(core-linux-transport-qt-event-in-out PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-linux-transport-qt-event-in-out PUBLIC
        Qt6::Core
        core-linux-transport-qt-event-base
        core-linux-transport-qt-event-targets
        core-linux-transport-qt-event-in
        core-linux-transport-qt-event-out
)

#if(BUILD_TESTING)
#    add_subdirectory(tests)
#endif()