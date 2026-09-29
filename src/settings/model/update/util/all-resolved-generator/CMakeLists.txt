cmake_minimum_required(VERSION 4.1)

project(settings-model-update-all-resolved-generator)

add_executable(settings-model-update-all-resolved-generator
    main.cpp
)

target_link_libraries(settings-model-update-all-resolved-generator PRIVATE
    settings-model-update-util-resolve-many
    settings-model-update-radios-basic-rx-lookup
    settings-model-update-radios-basic-rxtx-lookup
    settings-model-update-radios-basic-iq-rx-lookup
    settings-model-update-radios-basic-iq-rxtx-lookup
    settings-model-update-radios-dual-iq-rx-lookup
    settings-model-update-radios-dual-iq-rxtx-lookup
    settings-model-update-radios-dual-iq-split-lookup
)

target_include_directories(settings-model-update-all-resolved-generator PRIVATE
    ${INCLUDE_DIR}
)

# Pass the source directory to the executable via compile definition
target_compile_definitions(settings-model-update-all-resolved-generator PRIVATE
    SOURCE_PATH="${CMAKE_SOURCE_DIR}/src/settings/model/path"
)
