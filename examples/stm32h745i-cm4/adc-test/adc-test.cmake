cmake_minimum_required(VERSION 4.1)

project(example-cm4-adc-test)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

add_compile_definitions(USE_GPIO)

set(LD_FILE_CM4 ${CMAKE_CURRENT_LIST_DIR}/ld/STM32H745XIHX_FLASH.ld)

set(ADC_TEST_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/IqSink/MyIqSink.h
        ${CMAKE_CURRENT_LIST_DIR}/IqSink/MyIqSink.cpp

)

add_executable(example-cm4-adc-test
        ${ADC_TEST_SOURCES}
)

target_include_directories(example-cm4-adc-test PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(example-cm4-adc-test PUBLIC
        -T${LD_FILE_CM4}
#        config-struct
#        settings-model-core
#        settings-control-radio
#        settings-control-hal-digital
#        core-freertos-usb-host
        core-stm32h745i-setup-setup2-cm4
        core-stm32h745i-iq-source
)

target_link_options(example-cm4-adc-test PRIVATE -Wl,-Map=example-cm4-adc-test.map,--cref)

add_custom_command(TARGET example-cm4-adc-test POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:example-cm4-adc-test>
)

set(EXTERNAL_LOADER "${CMAKE_CURRENT_LIST_DIR}/MT25TL01G_STM32H745I-DISCO.stldr")

add_custom_target(FLASH_example-cm4-adc-test
        COMMAND STM32_Programmer_CLI --connect port=SWD --write ${CMAKE_BINARY_DIR}/example-cm4-adc-test.elf --start
        DEPENDS example-cm4-adc-test
)