cmake_minimum_required(VERSION 4.1)

project(core-common-transport-spectrum)

set(SPECTRUM_TRANSPORT_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/SpectrumPublisher.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/SpectrumClientSideRingsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/SpectrumMessageRingsT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/transport/SpectrumRadioSideRingsT.h
)

add_library(core-common-transport-spectrum INTERFACE ${SPECTRUM_TRANSPORT_FILES})

target_include_directories(core-common-transport-spectrum INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-common-transport-spectrum INTERFACE
        core-common-transport-meta
        settings-model-radio-iq-common
        settings-model-radio-iq-single-common
)

#if(BUILD_TESTING)
#    add_subdirectory(tests)
#endif()
