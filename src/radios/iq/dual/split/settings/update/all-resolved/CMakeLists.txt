cmake_minimum_required(VERSION 4.1)

project(radios-iq-dual-split-settings-update-resolved)

set(DUAL_IQ_SPLIT_RESOLVED_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/settings/update/SplitBandDualIqResolved.h
        ${CMAKE_CURRENT_LIST_DIR}/src/SplitBandDualIqResolved.cpp
)

add_library(radios-iq-dual-split-settings-update-resolved STATIC ${DUAL_IQ_SPLIT_RESOLVED_FILES})

target_include_directories(radios-iq-dual-split-settings-update-resolved PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_include_directories(radios-iq-dual-split-settings-update-resolved PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include/radios/settings/update
)

target_link_libraries(radios-iq-dual-split-settings-update-resolved PUBLIC
        settings-model-update-base
)