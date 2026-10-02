cmake_minimum_required(VERSION 4.1)

project(core-common-samples)

set(SAMPLE_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/samples/SampleTypes.h
        ${CMAKE_CURRENT_LIST_DIR}/include/samples/PingPongBuffers.h
)

add_library(core-common-samples INTERFACE ${SAMPLE_SOURCES})

target_compile_definitions(core-common-samples INTERFACE
        PIPELINE_BUFFER_LENGTH=4096
        FFT_SIZE=2048
)

target_include_directories(core-common-samples INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(core-common-samples INTERFACE etl::etl)