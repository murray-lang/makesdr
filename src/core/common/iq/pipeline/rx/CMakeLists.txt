cmake_minimum_required(VERSION 4.1)

project(core-common-iq-pipeline-rx)

set(PIPELINE_RX_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/pipeline/IqRxPipeline.h
        ${CMAKE_CURRENT_LIST_DIR}/src/IqRxPipeline.cpp
)

add_library(core-common-iq-pipeline-rx STATIC ${PIPELINE_RX_FILES})

target_include_directories(core-common-iq-pipeline-rx PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(PIPELINE_RX_LIBS
        core-common-iq-pipeline-base
        core-common-iq-pipeline-stage-correction
        core-common-iq-pipeline-stage-filter
        core-common-iq-pipeline-stage-modulation
        core-common-iq-pipeline-stage-resample
        core-common-iq-pipeline-stage-monitor
        core-common-iq-pipeline-stage-agc
        core-common-iq-pipeline-stage-s-meter
        core-common-transport-radio
)

#if(PLATFORM_FAMILY STREQUAL "linux")
#    set(PIPELINE_RX_LIBS
#            ${PIPELINE_RX_LIBS}
#            core-linux-event-data
#    )
#endif()

target_link_libraries(core-common-iq-pipeline-rx PUBLIC
        ${PIPELINE_RX_LIBS}

)