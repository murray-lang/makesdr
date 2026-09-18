cmake_minimum_required(VERSION 4.1)

project(core-common-iq-pipeline-base)

set(PIPELINE_BASE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/pipeline/IqPipeline.h
        ${CMAKE_CURRENT_LIST_DIR}/src/IqPipeline.cpp
)

add_library(core-common-iq-pipeline-base STATIC ${PIPELINE_BASE_FILES})

target_include_directories(core-common-iq-pipeline-base PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-common-iq-pipeline-base PUBLIC
        core-common-iq-base
        core-common-iq-io
        core-common-iq-pipeline-stage-base
        core-common-iq-pipeline-stage-oscillator
        core-common-audio-base
        settings-model-radios-iq-basic-common
        settings-model-data-radio
)