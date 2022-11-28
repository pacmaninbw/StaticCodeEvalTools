#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <fstream>
#include <filesystem>
#include <string>
#include "FileStatistics.h"
#include "StatisticsCollector.h"
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

static const std::string testRoot = "wconsteroids";

static bool FindUnitTestDirectorRoot(std::filesystem::path& testDir)
{
	testDir = std::filesystem::current_path();
	bool exitStatus = true;

	std::string MyPath = testDir.string();
	auto found = MyPath.find(testRoot);
	if (found == std::string::npos)
	{
		std::string failMessage = "The current directory \"" + testDir.string() +
			"\" is not within the \"" + testRoot + "\" directory structure\n";
		FAIL(failMessage);
		exitStatus = false;
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

	return exitStatus;
}

// The numbers used in testing are from running wconsteroids on
// the test file. With the exception of the widest line value
// they should match the core utils wc output. See the comment
// in the source code for why widest line value is different.
static constexpr std::size_t smallCharCount = 18304;
static constexpr std::size_t smallWidestLine = 182;
static constexpr std::size_t smallWordCount = 1687;
static constexpr std::size_t smallLineCount = 308;
static constexpr std::size_t bigCharCount = 89868;
static constexpr std::size_t bigWidestLine = 202;
static constexpr std::size_t bigWordCount = 7705;
static constexpr std::size_t bigLineCount = 1544;

TEST_CASE("Final Statistics Unit Test")
{
	// This test case should be run after all the other tests in this file
	// have passed.
	// These 2 sub tests will pass as long as the unitTest program is run
	// anywhere in the wconsteroids directory tree

	std::filesystem::path unitTestFile;
	bool found = false;

	found = FindUnitTestDirectorRoot(unitTestFile);

	SUBCASE("Smaller overall functionality test")
	{
		// Test the statistics collection against the source of CmdLineFileExtractor.h
		// and CmdLineFileExtractor.cpp as of the time of the creation of this test
		// case. The source of these 2 files is in smallerStatTest.txt.

		unitTestFile /= "smallerStatTest.txt";
		std::ifstream inStream(unitTestFile);

		if (!inStream.is_open())
		{
			std::string failMessage = "Can't open file \"" + unitTestFile.string() +
				"\" for input!\n";
			FAIL(failMessage);
		}

		FileStatistics smallerTestStat(unitTestFile.string());
		StatisticsCollector smallerTest(smallerTestStat);
		std::stringstream inputBuffer;
		inputBuffer << inStream.rdbuf();
		smallerTest.analyzeBuffer(inputBuffer.str());

		CHECK(smallerTestStat.getFileName().empty() == false);
		CHECK(smallerTestStat.getFileName() == unitTestFile);
		CHECK(smallerTestStat.getCharacters() == smallCharCount);
		CHECK(smallerTestStat.getWidestLine() == smallWidestLine);
		CHECK(smallerTestStat.getWords() == smallWordCount);
		CHECK(smallerTestStat.getTotalLines() == smallLineCount);
		inStream.close();
	}

	SUBCASE("Larger overall functionality test")
	{
		// Test the statistics collection against the source of both wconsteroids
		// and unitTest as of the time of the creation of this test case. All of
		// the source files are contained in bigStatisticsTest.txt.

		unitTestFile /= "bigStatisticsTest.txt";
		std::ifstream inStream(unitTestFile);

		if (!inStream.is_open())
		{
			std::string failMessage = "Can't open file \"" + unitTestFile.string() +
				"\" for input!\n";
			FAIL(failMessage);
		}

		FileStatistics biggerTestStat(unitTestFile.string());
		StatisticsCollector biggerTest(biggerTestStat);
		std::stringstream inputBuffer;
		inputBuffer << inStream.rdbuf();
		biggerTest.analyzeBuffer(inputBuffer.str());

		CHECK(biggerTestStat.getFileName().empty() == false);
		CHECK(biggerTestStat.getFileName() == unitTestFile);
		CHECK(biggerTestStat.getCharacters() == bigCharCount);
		CHECK(biggerTestStat.getWidestLine() == bigWidestLine);
		CHECK(biggerTestStat.getWords() == bigWordCount);
		CHECK(biggerTestStat.getTotalLines() == bigLineCount);
		inStream.close();
	}
}
