cmake_minimum_required(VERSION 4.1)

project(core-common-dsp-transforms-fft-pocket)

set(POCKET_FFT_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/include/dsp/transforms/fft/Fft.h
        ${CMAKE_CURRENT_LIST_DIR}/include/dsp/transforms/fft/pocketfft_hdronly.h
)

add_library(core-common-dsp-transforms-fft-pocket INTERFACE ${POCKET_FFT_SOURCES})

target_include_directories(core-common-dsp-transforms-fft-pocket INTERFACE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(core-common-dsp-transforms-fft-pocket INTERFACE
        core-common-samples
        core-common-dsp-window
)
