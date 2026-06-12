cmake_minimum_required(VERSION 3.25)

project(settings-model)

add_definitions(-DMAX_BANDS=34)

add_subdirectory(proto)
add_subdirectory(core)
add_subdirectory(extra)
add_subdirectory(meta)
add_subdirectory(util)

