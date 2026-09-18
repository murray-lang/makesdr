cmake_minimum_required(VERSION 4.1)

project(settings-model-radios-basic-common)

set(MODEL_BASIC_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/basic/BasicActiveBandSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/basic/BasicBandSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/basic/BasicBandSettingsCache.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/basic/WithBandT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/basic/WithModeT.h

        ${CMAKE_CURRENT_LIST_DIR}/src/BasicActiveBandSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/BasicBandSettings.cpp
)

add_library(settings-model-radios-basic-common STATIC ${MODEL_BASIC_FILES})

target_include_directories(settings-model-radios-basic-common PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-radios-basic-common PUBLIC
        settings-model-message
        settings-model-proto
        settings-model-data-radio
        settings-model-radios-base
)