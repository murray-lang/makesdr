cmake_minimum_required(VERSION 4.1)

project(core-common-iq-pipeline-stage-correction)

set(CORRECTION_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/correction/DcShift.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/correction/IqCorrection.h

        ${CMAKE_CURRENT_LIST_DIR}/src/DcShift.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/IqCorrection.cpp
)

add_library(core-common-iq-pipeline-stage-correction STATIC ${CORRECTION_FILES})

target_include_directories(core-common-iq-pipeline-stage-correction PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-common-iq-pipeline-stage-correction PUBLIC
        core-common-iq-pipeline-stage-base
        settings-model-radios-iq-basic-common
)
