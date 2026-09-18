cmake_minimum_required(VERSION 4.1)

project(core-common-iq-pipeline-stage-modulation)

set(MOD_FILES
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/modulation/AmDemodulator.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/modulation/CwDemodulator.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/modulation/CwModulator.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/modulation/Demodulator.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/modulation/FmDemodulator.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/modulation/FmModulator.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/modulation/Modulator.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/modulation/SsbDemodulator.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iq/modulation/SsbModulator.h

        ${CMAKE_CURRENT_LIST_DIR}/src/AmDemodulator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/CwDemodulator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/CwModulator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/FmDemodulator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/FmModulator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/SsbDemodulator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/SsbModulator.cpp


)

add_library(core-common-iq-pipeline-stage-modulation STATIC ${MOD_FILES})

target_include_directories(core-common-iq-pipeline-stage-modulation PUBLIC
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

if(USE_FREERTOS)
    target_link_libraries(core-common-iq-pipeline-stage-modulation PUBLIC
            core-common-samples
            settings-model-radios-base
            core-common-iq-pipeline-stage-oscillator
            core-freertos-liquid-dsp
    )
else()
    target_link_libraries(core-common-iq-pipeline-stage-modulation PUBLIC
            core-common-samples
            settings-model-radios-base
            core-common-iq-pipeline-stage-oscillator
            core-linux-liquid-dsp
    )
endif()

