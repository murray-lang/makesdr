cmake_minimum_required(VERSION 3.25)

project(core-sys-stm32h745i-drivers-hal)

set(STM32_HAL_SOURCES
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_conf.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_adc_ex.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_adc.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_cortex.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_dac_ex.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_dac.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_def.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_dma2d.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_dma_ex.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_dma.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_exti.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_flash_ex.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_flash.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_gpio_ex.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_gpio.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_hsem.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_i2c_ex.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_i2c.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_ltdc_ex.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_ltdc.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_mdma.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_mmc_ex.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_mmc.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_pcd_ex.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_pcd.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_pwr_ex.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_pwr.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_qspi.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_rcc_ex.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_rcc.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_sai_ex.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_sai.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_sd_ex.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_sd.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_sdram.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_tim_ex.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_tim.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_uart_ex.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_uart.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_usart_ex.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_hal_usart.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_ll_adc.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_ll_dac.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_ll_delayblock.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_ll_fmc.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_ll_sdmmc.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_ll_tim.h
        include/sys/stm32h745i/drivers/hal/stm32h7xx_ll_usb.h
        include/sys/stm32h745i/drivers/hal/Legacy/stm32_hal_legacy.h

        src/stm32h7xx_hal_adc.c
        src/stm32h7xx_hal_adc_ex.c
        src/stm32h7xx_hal.c
        src/stm32h7xx_hal_cortex.c
        src/stm32h7xx_hal_dac.c
        src/stm32h7xx_hal_dac_ex.c
        src/stm32h7xx_hal_dma2d.c
        src/stm32h7xx_hal_dma.c
        src/stm32h7xx_hal_dma_ex.c
        src/stm32h7xx_hal_exti.c
        src/stm32h7xx_hal_flash.c
        src/stm32h7xx_hal_flash_ex.c
        src/stm32h7xx_hal_gpio.c
        src/stm32h7xx_hal_hsem.c
        src/stm32h7xx_hal_i2c.c
        src/stm32h7xx_hal_i2c_ex.c
        src/stm32h7xx_hal_ltdc.c
        src/stm32h7xx_hal_ltdc_ex.c
        src/stm32h7xx_hal_mdma.c
        src/stm32h7xx_hal_mmc.c
        src/stm32h7xx_hal_mmc_ex.c
        src/stm32h7xx_hal_pcd.c
        src/stm32h7xx_hal_pcd_ex.c
        src/stm32h7xx_hal_pwr.c
        src/stm32h7xx_hal_pwr_ex.c
        src/stm32h7xx_hal_qspi.c
        src/stm32h7xx_hal_rcc.c
        src/stm32h7xx_hal_rcc_ex.c
        src/stm32h7xx_hal_sai.c
        src/stm32h7xx_hal_sai_ex.c
        src/stm32h7xx_hal_sd.c
        src/stm32h7xx_hal_sd_ex.c
        src/stm32h7xx_hal_sdram.c
        src/stm32h7xx_hal_tim.c
        src/stm32h7xx_hal_tim_ex.c
        src/stm32h7xx_hal_uart.c
        src/stm32h7xx_hal_uart_ex.c
        src/stm32h7xx_hal_usart.c
        src/stm32h7xx_hal_usart_ex.c
        src/stm32h7xx_ll_adc.c
        src/stm32h7xx_ll_dac.c
        src/stm32h7xx_ll_delayblock._c
        src/stm32h7xx_ll_fmc.c
        src/stm32h7xx_ll_sdmmc.c
        src/stm32h7xx_ll_tim.c
        src/stm32h7xx_ll_usb.c
)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    add_library(core-sys-stm32h745i-drivers-hal-cm7 STATIC ${STM32_HAL_SOURCES})
    target_include_directories(core-sys-stm32h745i-drivers-hal-cm7 PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}/include
    )
    target_link_libraries(core-sys-stm32h745i-drivers-hal-cm7 PUBLIC
            core-sys-stm32h745i-drivers-conf
            core-sys-stm32h745i-drivers-cmsis-cm7
    )
endif()

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm4")
    add_library(core-sys-stm32h745i-drivers-hal-cm4 STATIC ${STM32_HAL_SOURCES})
    target_include_directories(core-sys-stm32h745i-drivers-hal-cm4 PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}/include
    )
    target_link_libraries(core-sys-stm32h745i-drivers-hal-cm4 PUBLIC
            core-sys-stm32h745i-drivers-conf
            core-sys-stm32h745i-drivers-cmsis-cm4
    )
endif()