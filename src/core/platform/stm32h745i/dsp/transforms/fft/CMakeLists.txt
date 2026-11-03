cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-dsp-transforms-fft)

set(TRANSFORM_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/dsp/transforms/fft/Fft.h
)

if (TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    add_library(core-stm32h745i-cm4-dsp-transforms-fft INTERFACE ${TRANSFORM_SOURCES})

    target_include_directories(core-stm32h745i-cm4-dsp-transforms-fft INTERFACE
            ${INCLUDE_DIR}
            ${CMAKE_CURRENT_LIST_DIR}/include
    )

    target_link_libraries(core-stm32h745i-cm4-dsp-transforms-fft INTERFACE
            core-common-dsp-transforms-fft-base
            core-common-dsp-window
            core-stm32h745i-cm4-math
    )
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    add_library(core-stm32h745i-cm7-dsp-transforms-fft STATIC ${TRANSFORM_SOURCES})

    target_include_directories(core-stm32h745i-cm7-dsp-transforms-fft INTERFACE
            ${INCLUDE_DIR}
            ${CMAKE_CURRENT_LIST_DIR}/include
    )

    target_link_libraries(core-stm32h745i-cm7-dsp-transforms-fft INTERFACE
            core-common-samples
            core-common-dsp-window
            core-stm32h745i-cm7-math
    )
endif()
