cmake_minimum_required(VERSION 4.1)

project(settings-model-meta-radio)

set(SETTINGS_MODEL_LOOKUP_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/meta/radio/generalCoverageRadioLookup.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/meta/radio/RadioLookup.h
        ${CMAKE_CURRENT_LIST_DIR}/src/generalCoverageRadioLookup.cpp
)

add_library(settings-model-meta-radio STATIC ${SETTINGS_MODEL_LOOKUP_FILES})

target_include_directories(settings-model-meta-radio PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-meta-radio PUBLIC
        settings-model-meta-band
        settings-model-meta-mode
        settings-model-message
)
