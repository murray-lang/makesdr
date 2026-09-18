cmake_minimum_required(VERSION 4.1)

project(core-common-iq-base)

set(IQ_BASE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/base/IqSink.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/base/IqRxBaseT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/base/IqRxTxBaseT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/base/IqSource.h
)

add_library(core-common-iq-base INTERFACE ${IQ_BASE_SOURCES})
target_include_directories(core-common-iq-base INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-common-iq-base INTERFACE
        core-common-samples
        core-common-event
        config-struct
        settings-control-sink
        settings-model-radios-base
)