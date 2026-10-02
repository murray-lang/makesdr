cmake_minimum_required(VERSION 4.1)

project(core-common-iq-pipeline-stage-agc)

set(AGC_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/agc/AgcStage.h
        ${CMAKE_CURRENT_LIST_DIR}/src/AgcStage.cpp
)

add_library(core-common-iq-pipeline-stage-agc STATIC ${AGC_FILES})

target_include_directories(core-common-iq-pipeline-stage-agc PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set(AGC_LIBS
        core-common-iq-pipeline-stage-base
        settings-model-radio-base
)

if(PLATFORM_FAMILY STREQUAL "linux")
    set(AGC_LIBS
            ${AGC_LIBS}
            core-linux-liquid-dsp

    )
else()
    set(AGC_LIBS
            ${AGC_LIBS}
            core-freertos-liquid-dsp
    )
endif()

target_link_libraries(core-common-iq-pipeline-stage-agc PUBLIC
        ${AGC_LIBS}
)