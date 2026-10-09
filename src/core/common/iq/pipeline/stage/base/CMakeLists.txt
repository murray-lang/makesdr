cmake_minimum_required(VERSION 4.1)

project(core-common-iq-pipeline-stage-base)

add_library(core-common-iq-pipeline-stage-base INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/pipeline/stage/AgcGainDbProvider.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/pipeline/stage/IqPipelineStage.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/pipeline/stage/SampleRateProvider.h
)
target_include_directories(core-common-iq-pipeline-stage-base INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(core-common-iq-pipeline-stage-base INTERFACE core-common-samples)
