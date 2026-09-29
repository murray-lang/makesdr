cmake_minimum_required(VERSION 4.1)

project(settings-model-radio-basic-common)

set(MODEL_BASIC_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/basic/BasicActiveBandSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/basic/BasicBandSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/basic/BasicBandSettingsCache.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/basic/WithBandT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/basic/WithModeT.h

        ${CMAKE_CURRENT_LIST_DIR}/src/BasicActiveBandSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/BasicBandSettings.cpp
)

add_library(settings-model-radio-basic-common STATIC ${MODEL_BASIC_FILES})

target_include_directories(settings-model-radio-basic-common PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-radio-basic-common PUBLIC
        settings-model-message
        settings-model-proto
        settings-model-meta-radio
        settings-model-radio-base
        settings-model-update-base
)