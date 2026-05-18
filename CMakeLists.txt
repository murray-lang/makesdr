cmake_minimum_required(VERSION 4.1)

project(RadioSettingsProto)

set(CMAKE_CXX_STANDARD 20)

set(PROTO_PLATFORMS "nanopb" "google")
set(PROTO_PLATFORM "nanopb")

set(THIRD_PARTY_DIR  ${CMAKE_SOURCE_DIR}/third-party)
set(NANOPB_DIR "${THIRD_PARTY_DIR}/nanopb")
set(PROTO_DIR ${CMAKE_SOURCE_DIR}/proto)
set(GEN_DIR ${CMAKE_SOURCE_DIR}/gen)
set(GOOGLE_GEN_DIR ${GEN_DIR}/google)
set(NANOPB_GEN_DIR ${GEN_DIR}/nanopb)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(SETTINGS_DIR ${SRC_DIR}/settings)
set(ETL_DIR "${THIRD_PARTY_DIR}/etl")
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

list(APPEND CMAKE_MODULE_PATH ${NANOPB_DIR}/extra)

add_definitions(-DMAX_BANDS=34)

if (PROTO_PLATFORM STREQUAL "google")
    add_definitions(-DPROTO_GOOGLE)

    find_package(Protobuf REQUIRED)
elseif(PROTO_PLATFORM STREQUAL "nanopb")
    add_definitions(-DPROTO_NANOPB)

    find_package(nanopb REQUIRED)

    include("${NANOPB_DIR}/extra/FindNanopb.cmake")
endif()

if (PROTO_PLATFORM STREQUAL "google")
    add_library(proto_lib OBJECT ${PROTO_DIR}/RadioSettings.proto)
    target_link_libraries(proto_lib PUBLIC protobuf::libprotobuf)
    target_include_directories(proto_lib PUBLIC ${GOOGLE_GEN_DIR})

    protobuf_generate(
        TARGET proto_lib
        LANGUAGE cpp
        ${PROTO_DIR}/RadioSettings.proto
        PROTOC_OUT_DIR ${GOOGLE_GEN_DIR}
    )
elseif(PROTO_PLATFORM STREQUAL "nanopb")
#    set(NANOPB_GENERATE_CPP_APPEND_PATH OFF)
#    set(NANOPB_OPTIONS -I${PROTO_DIR} --output-dir=${NANOPB_GEN_DIR})
#    set(NANOPB_OPTIONS "--options-file=${PROTO_DIR}/RadioSettings.options")
    nanopb_generate_cpp(
            PROTO_SRCS PROTO_HDRS
#            TARGET nanopb_lib
#            RELPATH ${PROTO_DIR}
            ${PROTO_DIR}/RadioSettings.proto
    )
#    message(STATUS "Generated sources: ${PROTO_SRCS}")
#    message(STATUS "Generated headers: ${PROTO_HDRS}")
#    message(STATUS "NANOPB_OUT: ${NANOPB_OUT}")

    add_custom_target(copy_nanopb_gen ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/RadioSettings.pb.c ${NANOPB_GEN_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/RadioSettings.pb.h ${NANOPB_GEN_DIR}/
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/RadioSettings.pb.c ${CMAKE_CURRENT_BINARY_DIR}/RadioSettings.pb.h
        COMMENT "Copying nanopb generated files to source tree"
)
endif()

set(SETTINGS_SOURCES
    ${PROTO_SRCS}
    ${PROTO_HDRS}
    include/ResultCode.h
    src/main.cpp
#    src/settings/util/PbUtils.h
#    src/settings/util/PbUtils.cpp
        src/settings/band/ActiveBandSettings.h
        src/settings/band/BandSettings.h
        src/settings/band/BandSettingsCache.h
        src/settings/band/BandSettingsCache.cpp
        src/settings/band/SplitBandId.h
        src/settings/band/Band.h
        src/settings/band/BandList.h
        src/settings/band/BandList.cpp
        src/settings/band/AvailableBands.h
        src/settings/band/AvailableBands.cpp
        src/settings/band/BandCategory.h
        src/settings/band/BandCategoryList.h
        src/settings/band/BandCategoryList.cpp

        src/settings/base/SettingsBase.h
        src/settings/base/SettingFieldPath.h
        src/settings/base/SettingFieldVariant.h
        src/settings/base/SettingFieldUpdate.h
        src/settings/base/SteppableInt64Setting.h
        src/settings/base/SteppableFloatSetting.h
        src/settings/base/SteppableVariant.h

        src/settings/mode/Mode.h
        src/settings/mode/ModeList.h
        src/settings/mode/ModeList.cpp
        src/settings/mode/AvailableModes.h
        src/settings/mode/AvailableModes.cpp

        src/settings/util/MessageTagLookup.h
        src/settings/util/MessageTagLookup.cpp
        src/settings/util/GenerateResolvedPathSourceFiles.h
        src/settings/util/GenerateResolvedPathSourceFiles.cpp
        src/settings/util/ResolvedFieldPaths.h
        src/settings/util/ResolvedFieldPaths.cpp
        src/settings/util/DefaultRadioSettings.h
        src/settings/util/DefaultRadioSettings.cpp

        src/settings/pipeline/PipelineSettings.h
        src/settings/pipeline/IqCorrectionSettings.h
        src/settings/pipeline/MicrophoneSettings.h
        src/settings/pipeline/PipelineId.h
        src/settings/pipeline/AgcSpeed.h
        src/settings/pipeline/RfSettings.h
        src/settings/pipeline/IfSettings.h
        src/settings/pipeline/RxPipelineSettings.h

        src/settings/radio/RadioSettings.h
        src/settings/radio/RadioSettings.cpp
        src/settings/radio/AutoCompleteRadioSettings.h
        src/settings/radio/AutoCompleteRadioSettings.cpp
        src/settings/radio/ReceiverSettings.h
        src/settings/radio/TransmitterSettings.h
        src/settings/base/SettingsManipulator.cpp
        src/settings/base/SettingsManipulator.h

)

add_subdirectory(${ETL_DIR} etl)

add_executable(RadioSettingsProto ${SETTINGS_SOURCES})

if (PROTO_PLATFORM STREQUAL "google")
    target_link_libraries(RadioSettingsProto PRIVATE proto_lib)
elseif(PROTO_PLATFORM STREQUAL "nanopb")
    target_include_directories(RadioSettingsProto PUBLIC
            ${INCLUDE_DIR}
            ${SETTINGS_DIR}
            ${ETL_DIR}/include
            ${NANOPB_DIR}
            ${NANOPB_GEN_DIR}
    )
    add_dependencies(RadioSettingsProto copy_nanopb_gen)
endif()
