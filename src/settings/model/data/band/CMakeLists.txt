cmake_minimum_required(VERSION 4.1)

project(settings-model-data-band)

set(BAND_LOOKUP_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/data/band/BandCategoryList.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/data/band/BandList.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/data/band/mostBandCategories.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/data/band/BandTypes.h

        ${CMAKE_CURRENT_LIST_DIR}/src/BandList.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/mostBandCategories.cpp
)

add_library(settings-model-data-band STATIC ${BAND_LOOKUP_FILES})

target_include_directories(settings-model-data-band PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(settings-model-data-band PUBLIC
        settings-model-proto
        settings-model-message
        etl::etl
)