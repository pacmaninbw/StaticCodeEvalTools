# CMakeList.txt : CMake project for unitTest, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

# Add source to this project's executable.
add_executable (unitTest unitTest.cpp
    ../wconsteroids/CmdLineFileExtractor.cpp
    ../wconsteroids/CommandLineParser.cpp
    ../wconsteroids/FileProcessor.cpp
    ../wconsteroids/FileStatistics.cpp
    ../wconsteroids/ProgramOptions.cpp
    ../wconsteroids/ReportWriter.cpp
    ../wconsteroids/StatisticsCollector.cpp
    CmdLineFileExtractorTest.cpp
    CommandLineParserTest.cpp
    FileProcessorTest.cpp
    FileStatisticsTest.cpp
    ProgramOptionsTest.cpp
    ReportWriterTest.cpp
    StatisticsCollectorTest.cpp
)
target_include_directories(unitTest PUBLIC ../doctest ../wconsteroids)
target_compile_features(unitTest PUBLIC cxx_std_20)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # warning level 4 and all warnings as errors
    target_compile_options(unitTest PRIVATE /W4 /WX -D_CRT_SECURE_NO_WARNINGS)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # lots of warnings and all warnings as errors
    target_compile_options(unitTest PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

add_test(NAME unit_test COMMAND unitTest)

