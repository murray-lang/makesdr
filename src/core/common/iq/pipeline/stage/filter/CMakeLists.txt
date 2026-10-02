cmake_minimum_required(VERSION 4.1)

project(core-common-iq-pipeline-stage-filter)

find_package(PkgConfig)

find_package(Qt6 REQUIRED COMPONENTS Core) # Temporary for debugging with qDebug()

set(FILTER_STAGE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/filter/FilterStage.h

        ${CMAKE_CURRENT_LIST_DIR}/src/FilterStage.cpp
)

add_library(core-common-iq-pipeline-stage-filter STATIC ${FILTER_STAGE_FILES})

target_include_directories(core-common-iq-pipeline-stage-filter PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-common-iq-pipeline-stage-filter PUBLIC
        core-common-iq-pipeline-stage-base
        settings-model-radio-iq-basic-common
        core-common-dsp-filters-fir-custom
        Qt6::Core
)
