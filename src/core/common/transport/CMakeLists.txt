cmake_minimum_required(VERSION 4.1)

project(core-common-transport)

set(COMMON_TRANSPORT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/TransportBase.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/in/TransportInBaseT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/out/TransportOutBaseT.h
)

add_library(core-common-transport INTERFACE ${COMMON_TRANSPORT_FILES})

target_include_directories(core-common-transport INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-common-transport INTERFACE
        settings-model-message
)