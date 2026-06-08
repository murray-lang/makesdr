cmake_minimum_required(VERSION 3.25)

project(makesdr-settings-model-meta)

set(META_SOURCES
        include/settings/model/meta/RadioMeta.h
        include/settings/model/meta/BandCategoryList.h
        include/settings/model/meta/BandList.h
        include/settings/model/meta/basicModes.h
        include/settings/model/meta/generalCoverageRadioMeta.h
        include/settings/model/meta/ModeList.h
        include/settings/model/meta/mostBandCategories.h
        include/settings/model/meta/types.h

        src/BandCategoryList.cpp
        src/BandList.cpp
        src/basicModes.cpp
        src/generalCoverageRadioMeta.cpp
        src/ModeList.cpp
        src/mostBandCategories.cpp
)

add_library(makesdr-settings-model-meta STATIC ${META_SOURCES})

target_include_directories(makesdr-settings-model-meta PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(makesdr-settings-model-meta PUBLIC
        makesdr-settings-model-proto
        etl
)
