cmake_minimum_required(VERSION 4.1)

project(settings-model-radio-iq-dual-common)

set(MODEL_DUAL_IQ_COMPONENT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/DualIqActiveBandSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/DualIqBandSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/DualIqBandSettingsCache.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/RxTxDualIqActiveBandSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/RxTxDualIqBandSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/SplitBandDualIqActiveBandSettings.h

        ${CMAKE_CURRENT_LIST_DIR}/src/DualIqActiveBandSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/DualIqBandSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/RxTxDualIqActiveBandSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/RxTxDualIqBandSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/SplitBandDualIqActiveBandSettings.cpp
)

add_library(settings-model-radio-iq-dual-common STATIC ${MODEL_DUAL_IQ_COMPONENT_FILES})

target_include_directories(settings-model-radio-iq-dual-common PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-radio-iq-dual-common PUBLIC
        settings-model-radio-iq-basic-common
)