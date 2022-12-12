cmake_minimum_required(VERSION 3.15)

add_executable (wconsteroids
                main.cpp
                CmdLineFileExtractor.cpp
                CommandLineParser.cpp
                FileProcessor.cpp
                FileStatistics.cpp
                ProgramOptions.cpp
                ReportWriter.cpp
                StatisticsCollector.cpp
)

target_include_directories(wconsteroids PUBLIC ${DOCTEST_INCLUDE_DIR})
target_compile_features(wconsteroids PUBLIC cxx_std_20)
target_compile_definitions(wconsteroids PRIVATE DOCTEST_CONFIG_DISABLE)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # warning level 4 and all warnings as errors
    target_compile_options(wconsteroids PRIVATE /W4 /WX -D_CRT_SECURE_NO_WARNINGS)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # require at least gcc 12 to support ranges and views
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 12)
            message(FATAL_ERROR "GCC version must be at least 12!")
        endif()
    endif()
    # lots of warnings and all warnings as errors
    target_compile_options(wconsteroids PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

install(TARGETS wconsteroids CONFIGURATIONS Release RUNTIME)


