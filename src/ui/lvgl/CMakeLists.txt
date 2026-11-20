cmake_minimum_required(VERSION 4.1)

project(ui-lvgl)

if(TARGET_PLATFORM STREQUAL "stm32h745i-cm7")
    add_subdirectory(lib-ui)
    # Link the LVGL Editor's generated UI library against our LVGL port build.
    # Done here so the generated lvgl/CMakeLists.txt doesn't need editing.
    target_link_libraries(lib-ui PUBLIC core-stm32h745i-lvgl-cm7)
    if(TARGET lib-ui_user)
        target_link_libraries(lib-ui_user PUBLIC core-stm32h745i-lvgl-cm7)
    endif()

    add_subdirectory(RadioDisplay)
endif()
