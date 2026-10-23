cmake_minimum_required(VERSION 4.1)

project(core-common-transport-metering)

set(METERING_TRANSPORT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/RxMeteringClientSideRingsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/RxMeteringMessageRingsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/RxMeteringRadioSideRingsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/RxMeteringPublisher.h
 )

add_library(core-common-transport-metering INTERFACE ${METERING_TRANSPORT_FILES})

target_include_directories(core-common-transport-metering INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-common-transport-metering INTERFACE
        core-common-transport-base
        settings-model-radio-base
)

#if(BUILD_TESTING)
#    add_subdirectory(tests)
#endif()
