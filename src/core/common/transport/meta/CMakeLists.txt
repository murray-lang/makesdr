cmake_minimum_required(VERSION 4.1)

project(core-common-transport-meta)

set(META_TRANSPORT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/MetaClientSideRingsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/MetaMessageRingsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/MetaRadioSideRingsT.h
 )

add_library(core-common-transport-meta INTERFACE ${META_TRANSPORT_FILES})

target_include_directories(core-common-transport-meta INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-common-transport-meta INTERFACE
        core-common-transport-base
        settings-model-update-base
        settings-model-radio-base
        settings-model-meta-mode
        settings-model-meta-band
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
