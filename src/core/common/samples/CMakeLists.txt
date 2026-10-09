cmake_minimum_required(VERSION 4.1)

project(core-common-samples)

set(SAMPLE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/samples/SampleTypes.h
        ${CMAKE_CURRENT_LIST_DIR}/include/samples/PingPongBuffers.h
)

add_library(core-common-samples INTERFACE ${SAMPLE_SOURCES})

target_compile_definitions(core-common-samples INTERFACE
        PIPELINE_BUFFER_LENGTH=2048 # Currently this needs to be no smaller than FILTER_FFT_SIZE
        FILTER_FFT_SIZE=2048
        MONITOR_FFT_SIZE=1024
)

target_include_directories(core-common-samples INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-common-samples INTERFACE etl::etl)