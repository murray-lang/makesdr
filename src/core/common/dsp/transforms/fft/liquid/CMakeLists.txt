cmake_minimum_required(VERSION 4.1)

project(core-common-dsp-transforms-fft-liquid)

set(LIQUID_FFT_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/dsp/transforms/fft/Fft.h
)

add_library(core-common-dsp-transforms-fft-liquid INTERFACE ${LIQUID_FFT_SOURCES})

target_include_directories(core-common-dsp-transforms-fft-liquid INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

if(USE_FREERTOS)
    target_link_libraries(core-common-dsp-transforms-fft-liquid INTERFACE
            core-common-samples
            core-freertos-liquid-dsp
    )
else()
    target_link_libraries(core-common-dsp-transforms-fft-liquid INTERFACE
            core-common-samples
            core-common-dsp-window
            core-linux-liquid-dsp
    )
endif()