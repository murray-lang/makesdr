cmake_minimum_required(VERSION 4.1)

project(RadioConfigProto)

set(CMAKE_CXX_STANDARD 20)

set(PROTO_PLATFORMS "nanopb" "google")
set(PROTO_PLATFORM "nanopb")

set(NANOPB_DIR "$ENV{HOME}/dev/tools/nanopb")

list(APPEND CMAKE_MODULE_PATH ${NANOPB_DIR}/extra)

if (PROTO_PLATFORM STREQUAL "google")
    add_definitions(-DPROTO_GOOGLE)

    find_package(Protobuf REQUIRED)
elseif(PROTO_PLATFORM STREQUAL "nanopb")
    add_definitions(-DPROTO_NANOPB)

    include("${NANOPB_DIR}/extra/FindNanopb.cmake")
    find_package(nanopb REQUIRED)
endif()

set(PROTO_DIR ${CMAKE_SOURCE_DIR}/proto)
set(GEN_DIR ${CMAKE_SOURCE_DIR}/gen)
set(GOOGLE_GEN_DIR ${GEN_DIR}/google)
set(NANOPB_GEN_DIR ${GEN_DIR}/nanopb)

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
    src/main.cpp
    src/util/PbUtils.h
    src/util/PbUtils.cpp
    src/util/MessageTagLookup.h
    src/util/MessageTagLookup.cpp
)

add_executable(RadioConfigProto ${SETTINGS_SOURCES})

if (PROTO_PLATFORM STREQUAL "google")
    target_link_libraries(RadioConfigProto PRIVATE proto_lib)
elseif(PROTO_PLATFORM STREQUAL "nanopb")
    target_include_directories(RadioConfigProto PUBLIC
            ${INCLUDE_DIR}
            ${NANOPB_DIR}
            ${NANOPB_GEN_DIR}
    )
    add_dependencies(RadioConfigProto copy_nanopb_gen)
endif()
