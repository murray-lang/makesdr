cmake_minimum_required(VERSION 4.1)

project(core-common-iq-pipeline-stage-resample)

#find_package(PkgConfig)
#
#find_package(Qt6 REQUIRED COMPONENTS Core) # Temporary for debugging with qDebug()

set(RESAMPLE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/resample/Resampler.h
        ${CMAKE_CURRENT_LIST_DIR}/src/Resampler.cpp
)

add_library(core-common-iq-pipeline-stage-resample STATIC ${RESAMPLE_FILES})

target_include_directories(core-common-iq-pipeline-stage-resample PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

if(USE_FREERTOS)
    target_link_libraries(core-common-iq-pipeline-stage-resample PUBLIC
            core-common-iq-pipeline-stage-base
            core-freertos-liquid-dsp
    )
else()
    target_link_libraries(core-common-iq-pipeline-stage-resample PUBLIC
            core-common-iq-pipeline-stage-base
            core-linux-liquid-dsp
#            Qt6::Core
    )
endif()