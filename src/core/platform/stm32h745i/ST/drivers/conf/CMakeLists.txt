cmake_minimum_required(VERSION 4.1)

project(core-stm32h745i-drivers-conf)

set(STM32_DRIVERS_CONF_SOURCES
        include/stm32h745i/drivers/conf/ft5336_conf.h
        include/stm32h745i/drivers/conf/gt911_conf.h
        include/stm32h745i/drivers/conf/mt25tl01g_conf.h
        include/stm32h745i/drivers/conf/mt48lc4m32b2_conf.h
        include/stm32h745i/drivers/conf/stm32h745i_discovery_conf.h
)

add_library(core-stm32h745i-drivers-conf INTERFACE ${STM32_DRIVERS_CONF_SOURCES})

target_include_directories(core-stm32h745i-drivers-conf INTERFACE
        include
)