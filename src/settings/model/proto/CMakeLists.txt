cmake_minimum_required(VERSION 4.1)

project(settings-model-proto)

set(PROTO_PLATFORMS "nanopb" "google")
set(PROTO_PLATFORM "nanopb")

set(GEN_DIR ${CMAKE_CURRENT_LIST_DIR}/gen)
set(GOOGLE_GEN_DIR ${GEN_DIR}/google)
set(NANOPB_GEN_DIR ${GEN_DIR}/nanopb)
set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/proto)

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
    add_library(settings-model-proto OBJECT ${PROTO_DIR}/RadioSettings.proto)
    target_link_libraries(settings-model-proto PUBLIC protobuf::libprotobuf)
    target_include_directories(settings-model-proto PUBLIC ${GOOGLE_GEN_DIR})

    protobuf_generate(
            TARGET proto_lib
            LANGUAGE cpp
            ${CMAKE_CURRENT_LIST_DIR}/RadioSettings.proto
            ${CMAKE_CURRENT_LIST_DIR}/RadioPayloads.proto
            PROTOC_OUT_DIR ${GOOGLE_GEN_DIR}
    )
    add_dependencies(settings-model-proto proto_lib)
elseif(PROTO_PLATFORM STREQUAL "nanopb")


     nanopb_generate_cpp(
             PROTO_SRCS PROTO_HDRS
             ${CMAKE_CURRENT_LIST_DIR}/RadioSettings.proto
             ${CMAKE_CURRENT_LIST_DIR}/RadioPayloads.proto
     )

     add_custom_target(copy_nanopb_gen_settings ALL
             COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/RadioSettings.pb.c ${NANOPB_GEN_DIR}/
             COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/RadioSettings.pb.h ${PROJECT_INCLUDE_DIR}/
             DEPENDS ${PROTO_SRCS} ${PROTO_HDRS}
             COMMENT "Copying nanopb generated files to source tree"
     )

     add_custom_target(copy_nanopb_gen_message ALL
             COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/RadioPayloads.pb.c ${NANOPB_GEN_DIR}/
             COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/RadioPayloads.pb.h ${PROJECT_INCLUDE_DIR}/
             DEPENDS ${PROTO_SRCS} ${PROTO_HDRS}
             COMMENT "Copying nanopb generated files to source tree"
     )

    # Mark the copied files as generated so CMake doesn't expect them at configure time
    set_source_files_properties(
            ${NANOPB_GEN_DIR}/RadioSettings.pb.c
            ${NANOPB_GEN_DIR}/RadioPayloads.pb.c
            ${PROJECT_INCLUDE_DIR}/RadioSettings.pb.h
            ${PROJECT_INCLUDE_DIR}/RadioPayloads.pb.h
            PROPERTIES GENERATED TRUE
    )

    add_library(settings-model-proto STATIC
            include/settings/model/proto/ProtobufIo.h
            include/settings/model/proto/ProtoBuffer.h
            ${NANOPB_GEN_DIR}/RadioSettings.pb.c
            ${NANOPB_GEN_DIR}/RadioPayloads.pb.c
    )
    target_include_directories(settings-model-proto PUBLIC
            ${INCLUDE_DIR}
            ${CMAKE_CURRENT_LIST_DIR}/include
            ${NANOPB_DIR}
#            ${NANOPB_GEN_DIR}
    )

     target_include_directories(settings-model-proto PRIVATE
             ${CMAKE_CURRENT_LIST_DIR}/include/settings/model/proto
     )

    target_link_libraries(settings-model-proto PUBLIC
            protobuf-nanopb-static
    )
    add_dependencies(settings-model-proto copy_nanopb_gen_settings copy_nanopb_gen_message)

endif()
