cmake_minimum_required(VERSION 4.1)

project(settings-model-radios-iq-dual-common)

set(MODEL_DUAL_IQ_COMPONENT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/iq/DualIqActiveBandSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/iq/DualIqBandSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/iq/DualIqBandSettingsCache.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/iq/RxTxDualIqActiveBandSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/iq/RxTxDualIqBandSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/iq/SplitBandDualIqActiveBandSettings.h

        ${CMAKE_CURRENT_LIST_DIR}/src/DualIqActiveBandSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/DualIqBandSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/RxTxDualIqActiveBandSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/RxTxDualIqBandSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/SplitBandDualIqActiveBandSettings.cpp
)

add_library(settings-model-radios-iq-dual-common STATIC ${MODEL_DUAL_IQ_COMPONENT_FILES})

target_include_directories(settings-model-radios-iq-dual-common PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-radios-iq-dual-common PUBLIC
        settings-model-radios-iq-basic-common
)