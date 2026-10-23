cmake_minimum_required(VERSION 4.1)

project(settings-model-radio-iq-single-common)

set(MODEL_BASIC_IQ_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/SingleIqActiveBandSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/SingleIqBandSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/SingleIqBandSettingsCache.h


        ${CMAKE_CURRENT_LIST_DIR}/src/SingleIqActiveBandSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/SingleIqBandSettings.cpp
)

add_library(settings-model-radio-iq-single-common STATIC ${MODEL_BASIC_IQ_FILES})

target_include_directories(settings-model-radio-iq-single-common PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_include_directories(settings-model-radio-iq-single-common PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq
)

target_link_libraries(settings-model-radio-iq-single-common PUBLIC
        settings-model-message
        settings-model-meta-mode
        settings-model-radio-iq-common
        core-common-samples
)