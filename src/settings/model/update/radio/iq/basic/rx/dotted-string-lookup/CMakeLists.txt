cmake_minimum_required(VERSION 4.1)

project(settings-model-update-radios-basic-iq-rx-lookup)

set(BASIC_IQ_RX_TAG_LOOKUP_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/update/BasicIqRxTagLookup.h
        ${CMAKE_CURRENT_LIST_DIR}/src/BasicIqRxTagLookup.cpp
)

add_library(settings-model-update-radios-basic-iq-rx-lookup STATIC ${BASIC_IQ_RX_TAG_LOOKUP_FILES})

target_include_directories(settings-model-update-radios-basic-iq-rx-lookup PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-update-radios-basic-iq-rx-lookup PUBLIC
        settings-model-update-common
)