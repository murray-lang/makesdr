cmake_minimum_required(VERSION 4.1)

project(core-common-transport-iq)

set(IQ_TRANSPORT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/IqClientSideRingsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/IqPublisher.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/IqMessageRingsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/IqRadioSideRingsT.h
)

add_library(core-common-transport-iq INTERFACE ${IQ_TRANSPORT_FILES})

target_include_directories(core-common-transport-iq INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-common-transport-iq INTERFACE
        core-common-transport-base
        core-common-transport-radio
        settings-model-radio-iq-common
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
