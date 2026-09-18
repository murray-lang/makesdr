cmake_minimum_required(VERSION 4.1)

project(settings-model-data-radio)

set(SETTINGS_MODEL_LOOKUP_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/data/radio/generalCoverageRadioLookup.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/data/radio/RadioLookup.h
        ${CMAKE_CURRENT_LIST_DIR}/src/generalCoverageRadioLookup.cpp
)

add_library(settings-model-data-radio STATIC ${SETTINGS_MODEL_LOOKUP_FILES})

target_include_directories(settings-model-data-radio PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-data-radio PUBLIC
        settings-model-data-band
        settings-model-data-mode
        settings-model-message
)
