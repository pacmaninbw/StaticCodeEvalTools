cmake_minimum_required(VERSION 3.15)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # warning level 4 and all warnings as errors
    target_compile_options(wconsteroids PRIVATE /W4 /WX)
elsef(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # lots of warnings and all warnings as errors
    target_compile_options(wconsteroids PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

add_executable(wconsteroids
    CmdLineFileExtractor.cpp
    CommandLineParser.cpp
    FileProcessor.cpp
    FileStatistics.cpp
    main.cpp
    ProgramOptions.cpp
    ReportWriter.cpp
    StatisticsCollector.cpp
)
target_compile_features(wconsteroids PUBLIC cxx_std_20)
install(TARGETS wconsteroids CONFIGURATIONS Release RUNTIME)
