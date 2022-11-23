# CMakeList.txt : CMake project for wccmake, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (wconsteriods
		main.cpp
		CmdLineFileExtractor.cpp
		CommandLineParser.cpp
		FileProcessor.cpp
		FileStatistics.cpp
	    ProgramOptions.cpp
		ReportWriter.cpp
	    StatisticsCollector.cpp
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET wconsteriods PROPERTY CXX_STANDARD 20)
endif()


if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # warning level 4 and all warnings as errors
    target_compile_options(wconsteriods PRIVATE /W4 /WX)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # lots of warnings and all warnings as errors
    target_compile_options(wconsteriods PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

# TODO: Add tests and install targets if needed.
