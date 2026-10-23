cmake_minimum_required(VERSION 4.1)

project(settings-model-radio-iq-common)

set(MODEL_BASIC_IQ_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/SpectrumMessage.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/IqCorrectionSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/IqMessage.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/PipelineRfSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/PipelineSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/RxPipelineSettings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq/TxPipelineSettings.h


        ${CMAKE_CURRENT_LIST_DIR}/src/SpectrumMessage.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/IqMessage.cpp
)

add_library(settings-model-radio-iq-common STATIC ${MODEL_BASIC_IQ_FILES})

target_include_directories(settings-model-radio-iq-common PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_include_directories(settings-model-radio-iq-common PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/radio/iq
)

target_link_libraries(settings-model-radio-iq-common PUBLIC
        settings-model-message
        settings-model-meta-mode
        core-common-samples
)