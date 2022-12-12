# CMakeList.txt : CMake project for unitTest, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

# Add source to this project's executable.
add_executable (unitTest unitTest.cpp
    ${WCONSTEROIDS_INCLUDE_DIR}/CmdLineFileExtractor.cpp
    ${WCONSTEROIDS_INCLUDE_DIR}/CommandLineParser.cpp
    ${WCONSTEROIDS_INCLUDE_DIR}/FileProcessor.cpp
    ${WCONSTEROIDS_INCLUDE_DIR}/FileStatistics.cpp
    ${WCONSTEROIDS_INCLUDE_DIR}/ProgramOptions.cpp
    ${WCONSTEROIDS_INCLUDE_DIR}/ReportWriter.cpp
    ${WCONSTEROIDS_INCLUDE_DIR}/StatisticsCollector.cpp
    CmdLineFileExtractorTest.cpp
    CommandLineParserTest.cpp
    FileProcessorTest.cpp
    FileStatisticsTest.cpp
    ProgramOptionsTest.cpp
    ReportWriterTest.cpp
    StatisticsCollectorTest.cpp
)
target_include_directories(unitTest PUBLIC ${DOCTEST_INCLUDE_DIR} ${WCONSTEROIDS_INCLUDE_DIR})
target_compile_features(unitTest PUBLIC cxx_std_20)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # warning level 4 and all warnings as errors
    target_compile_options(unitTest PRIVATE /W4 /WX -D_CRT_SECURE_NO_WARNINGS)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # lots of warnings and all warnings as errors
    target_compile_options(unitTest PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

add_test(NAME unit_test COMMAND unitTest)

