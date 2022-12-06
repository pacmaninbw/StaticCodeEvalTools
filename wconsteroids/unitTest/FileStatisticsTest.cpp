#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <vector>
#include "FileStatistics.h"
#include "unitTest.h"
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

// Guess estimates of the ratio of code to comments and blank lines
// Also guess estimates of word count and characters per line
static constexpr std::size_t totalLineCount = 617;
static constexpr std::size_t commentLineCount =
static_cast<std::size_t>(totalLineCount * 0.09);
static constexpr std::size_t codeWithCommentCount =
static_cast<std::size_t>(totalLineCount * 0.01);
static constexpr std::size_t blankLineCount =
static_cast<std::size_t>(totalLineCount / 10);
static constexpr std::size_t codeLineCount = totalLineCount -
(commentLineCount + codeWithCommentCount + blankLineCount);
static constexpr std::size_t characterCount = totalLineCount * 51;
static constexpr std::size_t wordCount = totalLineCount * 7;
static constexpr std::size_t whiteSpaceCount = wordCount - 1;

TEST_CASE(FILE_STATISTICS_TEST_ID"1 FileStatistics Constructors")
{
	SUBCASE(FILE_STATISTICS_TEST_ID"1.1 Constructor with name")
	{
		std::string name("Constructor has name");
		FileStatistics constructorWithName(name);
		CHECK(constructorWithName.getFileName().empty() == false);
		CHECK(constructorWithName.getFileName() == name);
		CHECK(constructorWithName.getBlankLines() == 0);
		CHECK(constructorWithName.getCharacters() == 0);
		CHECK(constructorWithName.getCodeLines() == 0);
		CHECK(constructorWithName.getCodeWithComment() == 0);
		CHECK(constructorWithName.getCommentLines() == 0);
		CHECK(constructorWithName.getWhitespace() == 0);
		CHECK(constructorWithName.getWidestLine() == 0);
		CHECK(constructorWithName.getWords() == 0);
		CHECK(constructorWithName.getTotalLines() == 0);
		CHECK(constructorWithName.getPerecentageOfCode() == 0);
	}

	SUBCASE(FILE_STATISTICS_TEST_ID"1.2 Constructor with no name")
	{
		FileStatistics constructorWithNoName;
		CHECK(constructorWithNoName.getFileName().empty() == true);
		CHECK(constructorWithNoName.getBlankLines() == 0);
		CHECK(constructorWithNoName.getCharacters() == 0);
		CHECK(constructorWithNoName.getCodeLines() == 0);
		CHECK(constructorWithNoName.getCodeWithComment() == 0);
		CHECK(constructorWithNoName.getCommentLines() == 0);
		CHECK(constructorWithNoName.getWhitespace() == 0);
		CHECK(constructorWithNoName.getWidestLine() == 0);
		CHECK(constructorWithNoName.getWords() == 0);
		CHECK(constructorWithNoName.getTotalLines() == 0);
		CHECK(constructorWithNoName.getPerecentageOfCode() == 0);
	}
}

TEST_CASE(FILE_STATISTICS_TEST_ID"2 FileStatistics Increment Functions")
{
	FileStatistics testIncrement("Test Increment");

	// First test every increment function individually

	for (int i = 0; i < totalLineCount; i++)
	{
		testIncrement.incrementTotalLines();
	}
	CHECK(testIncrement.getTotalLines() == totalLineCount);

	for (int i = 0; i < commentLineCount; i++)
	{
		testIncrement.incrementCommentsLines();
	}
	CHECK(testIncrement.getCommentLines() == commentLineCount);

	for (int i = 0; i < codeWithCommentCount; i++)
	{
		testIncrement.incrementCodeWithComment();
	}
	CHECK(testIncrement.getCodeWithComment() == codeWithCommentCount);

	for (int i = 0; i < blankLineCount; i++)
	{
		testIncrement.incrementBlankLines();
	}
	CHECK(testIncrement.getBlankLines() == blankLineCount);

	for (int i = 0; i < codeLineCount; i++)
	{
		testIncrement.incrementCodeLines();
	}
	CHECK(testIncrement.getCodeLines() == codeLineCount);

	for (int i = 0; i < whiteSpaceCount; i++)
	{
		testIncrement.incrementWhitespace();
	}
	CHECK(testIncrement.getWhitespace() == whiteSpaceCount);

	for (int i = 0; i < characterCount; i++)
	{
		testIncrement.incrementCharacter();
	}
	CHECK(testIncrement.getCharacters() == characterCount);

	for (int i = 0; i < wordCount; i++)
	{
		testIncrement.incrementWords();
	}
	CHECK(testIncrement.getWords() == wordCount);

	// Now check to see if there were any side affects.
	CHECK(testIncrement.getTotalLines() == totalLineCount);
	CHECK(testIncrement.getCommentLines() == commentLineCount);
	CHECK(testIncrement.getCodeWithComment() == codeWithCommentCount);
	CHECK(testIncrement.getBlankLines() == blankLineCount);
	CHECK(testIncrement.getCodeLines() == codeLineCount);
	CHECK(testIncrement.getWhitespace() == whiteSpaceCount);
	CHECK(testIncrement.getCharacters() == characterCount);
	CHECK(testIncrement.getWords() == wordCount);
}

TEST_CASE(FILE_STATISTICS_TEST_ID"3 FileStatistics Widest Line")
{
	constexpr std::size_t widestLine = 85;
	std::vector<std::size_t> widths = {33, 32, 64, 73, widestLine, 21, 53, 42, 67, 33};

	FileStatistics WidestLine("widestLine");

	for (auto width : widths)
	{
		WidestLine.updateWidestLine(width);
	}

	CHECK(WidestLine.getWidestLine() == widestLine);
}

TEST_CASE(FILE_STATISTICS_TEST_ID"4 FileStatistics Code Percentage and Add Totals")
{
	FileStatistics testData("Percentage");
	testData.setCharCount(characterCount);
	testData.setToLineCount(totalLineCount);
	testData.addToWordCount(wordCount);
	for (int i = 0; i < commentLineCount; i++)
	{
		testData.incrementCommentsLines();
	}
	for (int i = 0; i < codeWithCommentCount; i++)
	{
		testData.incrementCodeWithComment();
	}
	for (int i = 0; i < blankLineCount; i++)
	{
		testData.incrementBlankLines();
	}
	for (int i = 0; i < codeLineCount; i++)
	{
		testData.incrementCodeLines();
	}
	for (int i = 0; i < whiteSpaceCount; i++)
	{
		testData.incrementWhitespace();
	}

	double percentage = static_cast<double>(
		static_cast<double>(codeLineCount) / static_cast<double>(totalLineCount));

	SUBCASE(FILE_STATISTICS_TEST_ID"4.1 Code Percentage")
	{
		CHECK(testData.getPerecentageOfCode() >= (percentage * 100) - 1);
		CHECK(testData.getPerecentageOfCode() <= (percentage * 100) + 1);
	}

	SUBCASE(FILE_STATISTICS_TEST_ID"4.2 Add Totals")
	{
		constexpr std::size_t multiplier = 6;
		FileStatistics totals("totals");

		for (int i = 0; i < multiplier; i++)
		{
			testData.addTotals(totals);
		}
		CHECK(totals.getTotalLines() == totalLineCount * multiplier);
		CHECK(totals.getCommentLines() == commentLineCount * multiplier);
		CHECK(totals.getCodeWithComment() == codeWithCommentCount * multiplier);
		CHECK(totals.getBlankLines() == blankLineCount * multiplier);
		CHECK(totals.getCodeLines() == codeLineCount * multiplier);
		CHECK(totals.getWhitespace() == whiteSpaceCount * multiplier);
		CHECK(totals.getCharacters() == characterCount * multiplier);
		CHECK(totals.getWords() == wordCount * multiplier);

		// The percentage should be the same as above
		CHECK(testData.getPerecentageOfCode() >= (percentage * 100) - 1);
		CHECK(testData.getPerecentageOfCode() <= (percentage * 100) + 1);
	}
}
