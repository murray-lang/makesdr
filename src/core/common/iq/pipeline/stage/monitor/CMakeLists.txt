cmake_minimum_required(VERSION 4.1)

project(core-common-iq-pipeline-stage-monitor)

set(IQ_MONITOR_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/monitor/MonitorStageT.h
)

add_library(core-common-iq-pipeline-stage-monitor INTERFACE ${IQ_MONITOR_FILES})

target_include_directories(core-common-iq-pipeline-stage-monitor INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(IQ_MONITOR_LIBS
        core-common-iq-pipeline-stage-base
)

#if(PLATFORM_FAMILY STREQUAL "linux")
#    set(IQ_MONITOR_LIBS
#            ${IQ_MONITOR_LIBS}
#            core-linux-event-data
#    )
#endif()

target_link_libraries(core-common-iq-pipeline-stage-monitor INTERFACE
        ${IQ_MONITOR_LIBS}
)