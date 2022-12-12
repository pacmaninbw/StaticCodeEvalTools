cmake_minimum_required(VERSION 3.15)
project(wconsteroids)

include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
    doctest
    PREFIX ${CMAKE_BINARY_DIR}/doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)

# Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(doctest source_dir)
set(DOCTEST_INCLUDE_DIR ${CMAKE_BINARY_DIR}/doctest/src/doctest/doctest CACHE INTERNAL "Path to include folder for doctest")
set(WCONSTEROIDS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/wconsteroids CACHE INTERNAL "Path to include folder for wconsteroids")

add_subdirectory(wconsteroids)
enable_testing()
add_subdirectory(unitTest)
