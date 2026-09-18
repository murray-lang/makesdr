set(GOOGLETEST_DIR ${CMAKE_SOURCE_DIR}/third-party/googletest)

if(NOT EXISTS ${GOOGLETEST_DIR}/CMakeLists.txt)
    message(FATAL_ERROR
            "googletest submodule not populated. Run: git submodule update --init third-party/googletest")
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
add_subdirectory(${GOOGLETEST_DIR} googletest EXCLUDE_FROM_ALL)

include(GoogleTest)
