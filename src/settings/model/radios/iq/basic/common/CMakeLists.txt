cmake_minimum_required(VERSION 4.1)

project(settings-model-radios-iq-basic-common)

set(MODEL_BASIC_IQ_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/iq/BasicIqActiveBandSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/iq/BasicIqBandSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/iq/BasicIqBandSettingsCache.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/iq/IqCorrectionSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/iq/PipelineSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/iq/PipelineRfSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/iq/RxPipelineSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radios/iq/TxPipelineSettings.h


        ${CMAKE_CURRENT_LIST_DIR}/src/BasicIqActiveBandSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/BasicIqBandSettings.cpp
)

add_library(settings-model-radios-iq-basic-common STATIC ${MODEL_BASIC_IQ_FILES})

target_include_directories(settings-model-radios-iq-basic-common PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-radios-iq-basic-common PUBLIC
        settings-model-message
        settings-model-data-mode
        settings-model-radios-basic-common
)