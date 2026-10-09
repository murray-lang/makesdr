cmake_minimum_required(VERSION 4.1)

project(core-common-dsp-transforms-power-spectrum)

set(POWER_SPECTRUM_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/dsp/transforms/power-spectrum/PowerSpectrumT.h
)

add_library(core-common-dsp-transforms-power-spectrum INTERFACE ${POWER_SPECTRUM_SOURCES})

target_include_directories(core-common-dsp-transforms-power-spectrum INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

if (TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    target_link_libraries(core-common-dsp-transforms-power-spectrum INTERFACE
            core-stm32h745i-cm4-dsp-transforms-fft
    )
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    target_link_libraries(core-common-dsp-transforms-power-spectrum INTERFACE
            core-stm32h745i-cm7-dsp-transforms-fft
    )
else()
    target_link_libraries(core-common-dsp-transforms-power-spectrum INTERFACE
            core-common-dsp-transforms-fft-pocket
    )
endif()