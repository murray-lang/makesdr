cmake_minimum_required(VERSION 3.25)

project(makesdr-example-restore)

add_executable(makesdr-example-restore
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

target_include_directories(makesdr-example-restore PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(makesdr-example-restore PRIVATE
        makesdr-settings-model-proto
        makesdr-settings-model-core
)