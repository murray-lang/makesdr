cmake_minimum_required(VERSION 4.1)

project(RadioSettingsProto-settings-model-mutable)

set(MUTABLE_SETTINGS_SOURCES
        #    ${CMAKE_CURRENT_LIST_DIR}/util/PbUtils.h
        #    ${CMAKE_CURRENT_LIST_DIR}/util/PbUtils.cpp
        src/BandList.cpp
        include/model/mutable/BandCategory.h
        include/model/mutable/BandCategoryList.h
        src/BandCategoryList.cpp

        ../core/include/model/core/SettingFieldUpdate.h
        include/model/mutable/SettingFieldUpdateSink.h
        include/model/mutable/SettingFieldUpdateSource.h


        include/model/mutable/MutableBandSettings.h
        src/MutableBandSettings.cpp
        include/model/mutable/MutableActiveBandSettings.h
        src/MutableActiveBandSettings.cpp
        #        ${CMAKE_CURRENT_LIST_DIR}/band/ResolvedActiveBandsFieldPaths.h
        #        ${CMAKE_CURRENT_LIST_DIR}/band/ResolvedActiveBandsFieldPaths.cpp
        include/model/mutable/BandSettingsCache.h
        src/BandSettingsCache.cpp


        include/model/mutable/ModeList.h
        src/ModeList.cpp


        #        ${CMAKE_CURRENT_LIST_DIR}/util/GenerateResolvedPathSourceFiles.h
        #        ${CMAKE_CURRENT_LIST_DIR}/util/GenerateResolvedPathSourceFiles.cpp
        #        ${CMAKE_CURRENT_LIST_DIR}/util/ResolvedFieldPaths.h
        #        ${CMAKE_CURRENT_LIST_DIR}/util/ResolvedFieldPaths.cpp

        include/model/mutable/MutablePipelineSettings.h
        src/MutablePipelineSettings.cpp
        include/model/mutable/MutableRxPipelineSettings.h
        include/model/mutable/MutableTxPipelineSettings.h

        include/model/mutable/MutableRadioSettings.h
        include/model/mutable/MutableRadioSettingsSink.h
        src/MutableRadioSettings.cpp

        include/model/mutable/defaultRadioCategories.h
        src/defaultRadioCategories.cpp
)

add_library(RadioSettingsProto-settings-model-mutable
        STATIC
        ${MUTABLE_SETTINGS_SOURCES}
)

target_include_directories(RadioSettingsProto-settings-model-mutable PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(RadioSettingsProto-settings-model-mutable PUBLIC
        RadioSettingsProto-settings-model-core
        RadioSettingsProto-settings-model-basic

)
