cmake_minimum_required(VERSION 4.1)

project(core-common-transport-updates)

set(UPDATES_TRANSPORT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/UpdateClientSideRingsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/UpdateMessageRingsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/UpdateRadioSideRingsT.h

 )

add_library(core-common-transport-updates INTERFACE ${UPDATES_TRANSPORT_FILES})

target_include_directories(core-common-transport-updates INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-common-transport-updates INTERFACE
        core-common-transport-base
        settings-model-update-base
)

#if(BUILD_TESTING)
#    add_subdirectory(tests)
#endif()
