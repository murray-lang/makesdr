cmake_minimum_required(VERSION 4.1)

project(core-common-iq-pipeline-stage-s-meter)

set(S_METER_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/s-meter/SMeterStage.h
        ${CMAKE_CURRENT_LIST_DIR}/src/SMeterStage.cpp
)

add_library(core-common-iq-pipeline-stage-s-meter STATIC ${S_METER_SOURCES})
target_include_directories(core-common-iq-pipeline-stage-s-meter PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-common-iq-pipeline-stage-s-meter PUBLIC
        core-common-samples
        core-common-dsp-constants
        core-common-transport-radio
        core-common-iq-pipeline-stage-base
)