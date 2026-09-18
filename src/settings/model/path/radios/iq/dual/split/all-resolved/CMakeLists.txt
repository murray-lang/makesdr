cmake_minimum_required(VERSION 4.1)

project(settings-model-path-radios-dual-iq-split-resolved)

set(DUAL_IQ_SPLIT_RESOLVED_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/path/SplitBandDualIqResolved.h
        ${CMAKE_CURRENT_LIST_DIR}/src/SplitBandDualIqResolved.cpp
)

add_library(settings-model-path-radios-dual-iq-split-resolved STATIC ${DUAL_IQ_SPLIT_RESOLVED_FILES})

target_include_directories(settings-model-path-radios-dual-iq-split-resolved PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_include_directories(settings-model-path-radios-dual-iq-split-resolved PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/path
)

target_link_libraries(settings-model-path-radios-dual-iq-split-resolved PUBLIC
        settings-model-message
)