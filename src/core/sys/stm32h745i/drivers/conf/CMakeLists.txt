cmake_minimum_required(VERSION 3.25)

project(core-sys-stm32h745i-drivers-conf)

set(STM32_DRIVERS_CONF_SOURCES
        include/sys/stm32h745i/drivers/conf/ft5336_conf.h
        include/sys/stm32h745i/drivers/conf/gt911_conf.h
        include/sys/stm32h745i/drivers/conf/mt25tl01g_conf.h
        include/sys/stm32h745i/drivers/conf/mt48lc4m32b2_conf.h
        include/sys/stm32h745i/drivers/conf/stm32h745i_discovery_conf.h
)

add_library(core-sys-stm32h745i-drivers-conf INTERFACE ${STM32_DRIVERS_CONF_SOURCES})

target_include_directories(core-sys-stm32h745i-drivers-conf INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
)