cmake_minimum_required(VERSION 3.25)

project(example-restore)

add_executable(example-restore
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

target_include_directories(example-restore PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(example-restore PRIVATE
        settings-model-proto
        settings-model-core
)