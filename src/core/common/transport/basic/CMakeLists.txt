cmake_minimum_required(VERSION 4.1)

project(core-common-transport-basic)

set(BASIC_TRANSPORT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/BasicClientSideRingsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/BasicMessageRingsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/BasicRadioSideRingsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/RxMeteringPublisher.h
 )

add_library(core-common-transport-basic INTERFACE ${BASIC_TRANSPORT_FILES})

target_include_directories(core-common-transport-basic INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-common-transport-basic INTERFACE
        core-common-transport-base
        settings-model-update-base
        settings-model-radio-base
        settings-model-meta-mode
        settings-model-meta-band
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
