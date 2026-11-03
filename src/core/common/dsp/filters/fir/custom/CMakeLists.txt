cmake_minimum_required(VERSION 4.1)

project(core-common-dsp-filters-fir-custom)

set(FIR_CUSTOM_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/dsp/filters/fir/Fir.h

        ${CMAKE_CURRENT_LIST_DIR}/include/dsp/filters/fir/kernels/BandPassFirKernel.h
#        ${CMAKE_CURRENT_LIST_DIR}/include/dsp/filters/fir/kernels/BandStopFirKernel.h
        ${CMAKE_CURRENT_LIST_DIR}/include/dsp/filters/fir/kernels/FirKernel.h
#        ${CMAKE_CURRENT_LIST_DIR}/include/dsp/filters/fir/kernels/LowPassFirKernel.h

        ${CMAKE_CURRENT_LIST_DIR}/src/BandPassFirKernel.cpp
#        ${CMAKE_CURRENT_LIST_DIR}/src/BandStopFirKernel.cpp
#        ${CMAKE_CURRENT_LIST_DIR}/src/LowPassFirKernel.cpp

)

add_library(core-common-dsp-filters-fir-custom STATIC ${FIR_CUSTOM_FILES})

target_include_directories(core-common-dsp-filters-fir-custom PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    target_link_libraries(core-common-dsp-filters-fir-custom PUBLIC
            core-common-dsp-constants
            core-common-dsp-window
            core-stm32h745i-cm4-dsp-transforms-fft
    )
elseif(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    target_link_libraries(core-common-dsp-filters-fir-custom PUBLIC
            core-common-dsp-constants
            core-common-dsp-window
            core-stm32h745i-cm7-dsp-transforms-fft
    )
else()
    target_link_libraries(core-common-dsp-filters-fir-custom PUBLIC
            core-common-dsp-constants
            core-common-dsp-window
            core-common-dsp-transforms-fft-pocket
    )
endif()