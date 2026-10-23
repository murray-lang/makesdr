cmake_minimum_required(VERSION 4.1)
#settings-model-update-radios-dual-split-lookup
project(settings-model-update-radios-dual-split-lookup)

set(DUAL_IQ_SPLIT_TAG_LOOKUP_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/update/SplitBandDualIqTagLookup.h
        ${CMAKE_CURRENT_LIST_DIR}/src/SplitBandDualIqTagLookup.cpp
)

add_library(settings-model-update-radios-dual-split-lookup STATIC ${DUAL_IQ_SPLIT_TAG_LOOKUP_FILES})

target_include_directories(settings-model-update-radios-dual-split-lookup PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-update-radios-dual-split-lookup PUBLIC
        settings-model-update-common
)