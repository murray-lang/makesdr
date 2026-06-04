cmake_minimum_required(VERSION 4.1)

project(makesdr-settings-model)

add_definitions(-DMAX_BANDS=34)

add_subdirectory(proto)
add_subdirectory(core)
add_subdirectory(extra)
add_subdirectory(util)
