cmake_minimum_required(VERSION 4.1)

project(settings-model)

add_definitions(-DMAX_BANDS=34)

add_subdirectory(proto)
add_subdirectory(data)
add_subdirectory(path)
add_subdirectory(radios)
#add_subdirectory(util)
add_subdirectory(message)

