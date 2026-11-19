cmake_minimum_required(VERSION 4.1)

project(example-cm4-single-iq-rx)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

add_compile_definitions(USE_GPIO)

set(LD_FILE_CM4 ${CMAKE_CURRENT_LIST_DIR}/ld/STM32H745XIHX_FLASH.ld)

set(SINGLE_IQ_RX_SOURCES
        # Audio test code, superseded by SaiAudio (core-stm32h745i-audio). Out of the build because
        # audioLoopbackTask.cpp defines the BSP_AUDIO_OUT_* callbacks that SaiAudio.cpp now owns.
#        ${CMAKE_CURRENT_LIST_DIR}/audio/audio.cpp
#        ${CMAKE_CURRENT_LIST_DIR}/audio/audio.h
#        ${CMAKE_CURRENT_LIST_DIR}/audio/audioLoopbackTask.cpp
#        ${CMAKE_CURRENT_LIST_DIR}/audio/audioLoopbackTask.h

        ${CMAKE_CURRENT_LIST_DIR}/config/digitalInputConfigs.cpp
        ${CMAKE_CURRENT_LIST_DIR}/config/digitalInputConfigs.h
        ${CMAKE_CURRENT_LIST_DIR}/config/radioConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/config/radioConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
#        ${CMAKE_CURRENT_LIST_DIR}/IqSink/MyIqSink.h
#        ${CMAKE_CURRENT_LIST_DIR}/IqSink/MyIqSink.cpp

)

add_executable(example-cm4-single-iq-rx
        ${SINGLE_IQ_RX_SOURCES}
)

target_include_directories(example-cm4-single-iq-rx PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(example-cm4-single-iq-rx PUBLIC
        -T${LD_FILE_CM4}
        config-struct
        core-stm32h745i-setup-setup3-cm4
        radios-iq-single-rx
        core-stm32h745i-cm4-tinyusb-host
        radios-iq-single-rx-tests-settings
#        core-stm32h745i-iq-source
)

target_link_options(example-cm4-single-iq-rx PRIVATE -Wl,-Map=example-cm4-single-iq-rx.map,--cref)

add_custom_command(TARGET example-cm4-single-iq-rx POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:example-cm4-single-iq-rx>
)

set(EXTERNAL_LOADER "${CMAKE_CURRENT_LIST_DIR}/MT25TL01G_STM32H745I-DISCO.stldr")

add_custom_target(FLASH_example-cm4-single-iq-rx
        COMMAND STM32_Programmer_CLI --connect port=SWD --write ${CMAKE_BINARY_DIR}/example-cm4-single-iq-rx.elf --start
        DEPENDS example-cm4-single-iq-rx
)