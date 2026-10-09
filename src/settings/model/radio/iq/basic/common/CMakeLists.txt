cmake_minimum_required(VERSION 4.1)

project(settings-model-radio-iq-basic-common)

set(MODEL_BASIC_IQ_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/BasicIqActiveBandSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/BasicIqBandSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/BasicIqBandSettingsCache.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/FftMessage.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/IqCorrectionSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/IqMessage.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/PipelineSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/PipelineRfSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/RxPipelineSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/TxPipelineSettings.h


        ${CMAKE_CURRENT_LIST_DIR}/src/BasicIqActiveBandSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/BasicIqBandSettings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/FftMessage.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/IqMessage.cpp
)

add_library(settings-model-radio-iq-basic-common STATIC ${MODEL_BASIC_IQ_FILES})

target_include_directories(settings-model-radio-iq-basic-common PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_include_directories(settings-model-radio-iq-basic-common PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq
)

target_link_libraries(settings-model-radio-iq-basic-common PUBLIC
        settings-model-message
        settings-model-meta-mode
        settings-model-radio-basic-common
        core-common-samples
)