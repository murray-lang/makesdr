cmake_minimum_required(VERSION 4.1)

project(RadioSettingsProto-settings-model-proto)

set(PROTO_PLATFORMS "nanopb" "google")
set(PROTO_PLATFORM "nanopb")

set(GEN_DIR ${CMAKE_CURRENT_LIST_DIR}/gen)
set(GOOGLE_GEN_DIR ${GEN_DIR}/google)
set(NANOPB_GEN_DIR ${GEN_DIR}/nanopb)

list(APPEND CMAKE_MODULE_PATH ${NANOPB_DIR}/extra)

message(${NANOPB_DIR})

if (PROTO_PLATFORM STREQUAL "google")
    add_definitions(-DPROTO_GOOGLE)

    find_package(Protobuf REQUIRED)
elseif(PROTO_PLATFORM STREQUAL "nanopb")
    add_definitions(-DPROTO_NANOPB)

    # Include FindNanopb.cmake for nanopb_generate_cpp() function
    include("${NANOPB_DIR}/extra/FindNanopb.cmake")
endif()

if (PROTO_PLATFORM STREQUAL "google")
    add_library(RadioSettingsProto-settings-model-proto OBJECT ${PROTO_DIR}/RadioSettings.proto)
    target_link_libraries(RadioSettingsProto-settings-model-proto PUBLIC protobuf::libprotobuf)
    target_include_directories(RadioSettingsProto-settings-model-proto PUBLIC ${GOOGLE_GEN_DIR})

    protobuf_generate(
            TARGET proto_lib
            LANGUAGE cpp
            ${CMAKE_CURRENT_LIST_DIR}/RadioSettings.proto
            PROTOC_OUT_DIR ${GOOGLE_GEN_DIR}
    )
    add_dependencies(RadioSettingsProto-settings-model-proto proto_lib)
elseif(PROTO_PLATFORM STREQUAL "nanopb")


    #    set(NANOPB_GENERATE_CPP_APPEND_PATH OFF)
    #    set(NANOPB_OPTIONS -I${PROTO_DIR} --output-dir=${NANOPB_GEN_DIR})
    #    set(NANOPB_OPTIONS "--options-file=${PROTO_DIR}/RadioSettings.options")
    nanopb_generate_cpp(
            PROTO_SRCS PROTO_HDRS
            #            TARGET nanopb_lib
            #            RELPATH ${PROTO_DIR}
            ${CMAKE_CURRENT_LIST_DIR}/RadioSettings.proto
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

    add_library(RadioSettingsProto-settings-model-proto STATIC
            include/model/proto/RadioSettings.pb.h
            ${NANOPB_GEN_DIR}/RadioSettings.pb.c
    )
    target_include_directories(RadioSettingsProto-settings-model-proto PUBLIC
            ${INCLUDE_DIR}
            ${CMAKE_CURRENT_LIST_DIR}/include
            ${NANOPB_DIR}
            ${NANOPB_GEN_DIR}
    )

    target_link_libraries(RadioSettingsProto-settings-model-proto PUBLIC
            protobuf-nanopb-static
    )
    add_dependencies(RadioSettingsProto-settings-model-proto copy_nanopb_gen)

endif()
