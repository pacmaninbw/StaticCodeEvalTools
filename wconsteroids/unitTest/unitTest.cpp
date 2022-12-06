//#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4007) // 'function' : must be 'attribute' - see issue #182
DOCTEST_MSVC_SUPPRESS_WARNING_POP

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include "Executionctrlvalues.h"
#include "CmdLineFileExtractor.h"
#include "CommandLineParser.h"
#include "FileProcessor.h"
#include "FileStatistics.h"
#include "ProgramOptions.h"
#include "ReportWriter.h"
#include "StatisticsCollector.h"
#include "unitTest.h"
#include "UtilityTimer.h"
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

static const std::string testRoot = "wconsteroids";

/*
 * This function is used in several parts of the program, it allows testing
 * using files as input. The unit test program will not run outside the
 * source tree.
 */
std::string FindUnitTestDirectorRoot()
{
	std::filesystem::path testDir = std::filesystem::current_path();

	std::string testingPath = testDir.string();
	auto found = testingPath.find(testRoot);
	if (found == std::string::npos)
	{
		std::string failMessage = "The current directory \"" + testDir.string() +
			"\" is not within the \"" + testRoot + "\" directory structure\n";
		std::cerr << "FATAL ERROR: " << failMessage;
		exit(EXIT_FAILURE);
	}
	else
	{
		while (testDir.filename() != testRoot)
		{
			testDir = testDir.parent_path();
		}
		// Due to how the wconsteroids was first created with visual studio
		// there are 2 wconsteroids directories, the top one is the one we
		// are looking for.
		if (testDir.parent_path().filename() == testRoot)
		{
			testDir = testDir.parent_path();
		}
		testDir /= "unitTest";
	}

	return testDir.string();
}

int main(int argc, char** argv)
{
	// If the test is not being run in the source directory tree some of the
	// tests will fail. The the user and quit.
	std::string testPath = FindUnitTestDirectorRoot();
	if (testPath.empty())
	{
		return EXIT_FAILURE;
	}

	doctest::Context context;

	// The tests are ordered A through Z, the order builds from the simplest building
	// blocks to agregated assemblies, testing the least complex code first.
	context.setOption("order-by", "name");

	context.applyCommandLine(argc, argv);

	int res = context.run(); // run

	return res;
}