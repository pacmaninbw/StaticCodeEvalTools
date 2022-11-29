#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <fstream>
#include <filesystem>
#include <string>
#include <string_view>
#include "FileStatistics.h"
#include "StatisticsCollector.h"
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

// This vector is input for the first 2 
static std::vector<std::string_view> oneLineTestInput =
{
	"\n",												// 1 char, 1 white space no words
	"Hello World\n",									// 12 char, 2 white space, 2 words
	"Hello	World	\n",								// 2 tabs, 27 char, 3 white space, 2 words
	"Very simple test, not code, no tabs\n",			// 36 char, 7 white space including new line , 7 words
	"				\n",								// 4 tabs, 33 char including new line, 5 white space, no words
	"               \n",								// no tabs, 16 char, 16 white space, no words
	"This will be the widest line								\n", //8 tabs, // 93 char, 14 white space, 6 words
};

struct Test1ExpectedResults
{
	std::size_t charCount;
	std::size_t wordCount;
	std::size_t whiteSpaceCount;
	std::size_t width;
};

static std::vector<struct Test1ExpectedResults> testOneExpectedValues =
{
	{1, 0, 1, 1},
	{12, 2, 2, 12},
	{13, 2, 3, 29},
	{36, 7, 7, 36},
	{5, 0, 5, 37},
	{17, 0, 16, 16},
	{37, 6, 14, 101},
};
constexpr std::size_t widestLineIndex = 6;

static std::string mergeOneLineTests(Test1ExpectedResults& totals)
{
	std::size_t testIndex = 0;

	std::string mergedTestInputStr;
	for (auto line : oneLineTestInput)
	{
		mergedTestInputStr += line;
		totals.wordCount += testOneExpectedValues[testIndex].wordCount;
		totals.whiteSpaceCount += testOneExpectedValues[testIndex].whiteSpaceCount;
		totals.charCount += testOneExpectedValues[testIndex].charCount;
		testIndex++;
	}

	totals.width = testOneExpectedValues[widestLineIndex].width;

	return mergedTestInputStr;
}

TEST_CASE("Statistics Collection Test 1: Count Words and White Space")
{
	SUBCASE("Sub Test 1: Test One Liners")
	{
		std::size_t testIndex = 0;
		for (auto line : oneLineTestInput)
		{
			FileStatistics FSTestOne("one line");
			StatisticsCollector SCTestOne(FSTestOne);
			SCTestOne.countWordsAndWhiteSpace(line);
			CHECK(FSTestOne.getWords() == testOneExpectedValues[testIndex].wordCount);
			CHECK(FSTestOne.getWhitespace() == testOneExpectedValues[testIndex].whiteSpaceCount);
			testIndex++;
		}
	}

	SUBCASE("Sub Test 2: All One Liners Together")
	{
		Test1ExpectedResults totals = { 0 };
		std::string temp = mergeOneLineTests(totals);
		std::string_view mergedTestInput = temp;

		FileStatistics FSTestOne("merged test");
		StatisticsCollector SCTestOne(FSTestOne);
		SCTestOne.countWordsAndWhiteSpace(mergedTestInput);
		CHECK(FSTestOne.getWords() == totals.wordCount);
		CHECK(FSTestOne.getWhitespace() == totals.whiteSpaceCount);
	}
}

TEST_CASE("Statistics Collection Test 2: Widest Line")
{
	SUBCASE("Sub Test 3: Test Widest One Liners")
	{
		std::size_t testIndex = 0;
		for (auto line : oneLineTestInput)
		{
			FileStatistics FSTestOne("one line");
			StatisticsCollector SCTestOne(FSTestOne);
			auto endBuffer = line.end();
			auto startBuffer = line.begin();

			SCTestOne.updateWidestLine(startBuffer, endBuffer);
			CHECK(FSTestOne.getWidestLine() == testOneExpectedValues[testIndex].width);
			testIndex++;
		}
	}

	SUBCASE("Sub Test 4: Test Widest Line Merged")
	{
		Test1ExpectedResults totals = { 0 };
		std::string temp = mergeOneLineTests(totals);
		std::string_view mergedTestInput = temp;

		FileStatistics FSTestOne("one line");
		StatisticsCollector SCTestOne(FSTestOne);
		auto endBuffer = mergedTestInput.end();
		auto startBuffer = mergedTestInput.begin();

		auto currentChar = startBuffer;
		while (currentChar != endBuffer)
		{
			SCTestOne.updateWidestLine(currentChar, endBuffer);
		}

		CHECK(FSTestOne.getWidestLine() == testOneExpectedValues[widestLineIndex].width);
	}
}

TEST_CASE("Statistics Collection Test 3: Analyze Buffer")
{
	SUBCASE("Sub Test 5: Analyze Buffer Test One Liners")
	{
		std::size_t testIndex = 0;
		for (auto line : oneLineTestInput)
		{
			FileStatistics FSTestOne("one line");
			StatisticsCollector SCTestOne(FSTestOne);
			SCTestOne.analyzeBuffer(line);
			CHECK(FSTestOne.getCharacters() == testOneExpectedValues[testIndex].charCount);
			CHECK(FSTestOne.getWords() == testOneExpectedValues[testIndex].wordCount);
			CHECK(FSTestOne.getWhitespace() == testOneExpectedValues[testIndex].whiteSpaceCount);
			CHECK(FSTestOne.getWidestLine() == testOneExpectedValues[testIndex].width);
			CHECK(FSTestOne.getTotalLines() == 1);
			testIndex++;
		}
	}

	SUBCASE("Sub Test 6: Analyze Buffer Merged Test")
	{
		Test1ExpectedResults totals = { 0 };
		std::string temp = mergeOneLineTests(totals);
		std::string_view mergedTestInput = temp;

		FileStatistics FSTestOne("one line");
		StatisticsCollector SCTestOne(FSTestOne);
		SCTestOne.analyzeBuffer(mergedTestInput);
		CHECK(FSTestOne.getCharacters() == totals.charCount);
		CHECK(FSTestOne.getWords() == totals.wordCount);
		CHECK(FSTestOne.getWhitespace() == totals.whiteSpaceCount);
		CHECK(FSTestOne.getWidestLine() == totals.width);
		CHECK(FSTestOne.getTotalLines() == testOneExpectedValues.size());
	}
}

/*
 * Final StatisticsCollector Unit Test
 * Use actual source files to test the statistics collection.
 */
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
