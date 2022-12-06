#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>
#include "ProgramOptions.h"
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

static void checkWhatFailed(ProgramOptions expectedData, ProgramOptions testData)
{
	CHECK(expectedData.blankLineCount == testData.blankLineCount);
	CHECK(expectedData.byteCount == testData.byteCount);
	CHECK(expectedData.charCount == testData.charCount);
	CHECK(expectedData.codeCount == testData.codeCount);
	CHECK(expectedData.commentCount == testData.commentCount);
	CHECK(expectedData.enableExecutionTime == testData.enableExecutionTime);
	CHECK(expectedData.lineCount == testData.lineCount);
	CHECK(expectedData.maxLineWidth == testData.maxLineWidth);
	CHECK(expectedData.percentages == testData.percentages);
	CHECK(expectedData.recurseSubDirectories == testData.recurseSubDirectories);
	CHECK(expectedData.whitespaceCount == testData.whitespaceCount);
	CHECK(expectedData.wordCount == testData.wordCount);
}

TEST_CASE("Test D Program Options: Constructor")
{
	ProgramOptions testConstructor;
	CHECK(testConstructor.blankLineCount == false);
	CHECK(testConstructor.byteCount == false);
	CHECK(testConstructor.charCount == false);
	CHECK(testConstructor.codeCount == false);
	CHECK(testConstructor.commentCount == false);
	CHECK(testConstructor.enableExecutionTime == false);
	CHECK(testConstructor.lineCount == false);
	CHECK(testConstructor.maxLineWidth == false);
	CHECK(testConstructor.percentages == false);
	CHECK(testConstructor.recurseSubDirectories == false);
	CHECK(testConstructor.whitespaceCount == false);
	CHECK(testConstructor.wordCount == false);
	CHECK(testConstructor.version == "1.0.0");
	CHECK(testConstructor.useDefaultFlags == true);
}

TEST_CASE("Test E Program Options: Set Defaults When No Flags")
{
	ProgramOptions expectData;
	expectData.byteCount = true;
	expectData.wordCount = true;
	expectData.lineCount = true;

	ProgramOptions testData;
	testData.SetDefaultOptionsWhenNoFlags();
	CHECK(expectData == testData);
	if (expectData != testData)
	{
		checkWhatFailed(expectData, testData);
	}
}

TEST_CASE("Test F Program Options: Single Dash Arguments")
{
	SUBCASE("Single Dash Arguments Sub Test 1")
	{
		// When a single output flag is set, only that output should be expected
		ProgramOptions expectData;
		std::string_view inputData = "-L";
		expectData.maxLineWidth = true;
		expectData.useDefaultFlags = false;

		ProgramOptions testData;
		testData.processSingleDashOptions(inputData);
		testData.SetDefaultOptionsWhenNoFlags();
		CHECK(expectData == testData);
		if (expectData != testData)
		{
			checkWhatFailed(expectData, testData);
		}
	}

	SUBCASE("Single Dash Arguments Sub Test 2")
	{
		// -R, recursive file search should not affect default flags
		ProgramOptions expectData;
		std::string_view inputData = "-R";
		expectData.recurseSubDirectories = true;
		expectData.byteCount = true;
		expectData.lineCount = true;
		expectData.wordCount = true;

		ProgramOptions testData;
		testData.processSingleDashOptions(inputData);
		testData.SetDefaultOptionsWhenNoFlags();
		CHECK(expectData == testData);
		if (expectData != testData)
		{
			checkWhatFailed(expectData, testData);
		}
	}

	SUBCASE("Single Dash Arguments Sub Test 3")
	{
		// -t, time execution should not affect default flags
		ProgramOptions expectData;
		std::string_view inputData = "-t";
		expectData.enableExecutionTime = true;
		expectData.byteCount = true;
		expectData.lineCount = true;
		expectData.wordCount = true;

		ProgramOptions testData;
		testData.processSingleDashOptions(inputData);
		testData.SetDefaultOptionsWhenNoFlags();
		CHECK(expectData == testData);
		if (expectData != testData)
		{
			checkWhatFailed(expectData, testData);
		}
	}

	SUBCASE("Single Dash Arguments Sub Test 4: Multiple Flags")
	{
		ProgramOptions expectData;
		std::string_view inputData = "-RtLlc";
		expectData.enableExecutionTime = true;
		expectData.recurseSubDirectories = true;
		expectData.byteCount = true;
		expectData.lineCount = true;
		expectData.maxLineWidth = true;
		expectData.useDefaultFlags = false;

		ProgramOptions testData;
		testData.processSingleDashOptions(inputData);
		testData.SetDefaultOptionsWhenNoFlags();
		CHECK(expectData == testData);
		if (expectData != testData)
		{
			checkWhatFailed(expectData, testData);
		}
	}
}

TEST_CASE("Test G Program Options: Double Dash Arguments")
{
	SUBCASE("Double Dash Arguments Sub Test 1")
	{
		// When a single output flag is set, only that output should be expected
		ProgramOptions expectData;
		std::string_view inputData = "--bytes";
		expectData.byteCount = true;
		expectData.useDefaultFlags = false;

		ProgramOptions testData;
		testData.processDoubleDashOptions(inputData);
		testData.SetDefaultOptionsWhenNoFlags();
		CHECK(expectData == testData);
		if (expectData != testData)
		{
			checkWhatFailed(expectData, testData);
		}
	}

	SUBCASE("Double Dash Arguments Sub Test 2")
	{
		// -R, recursive file search should not affect default flags
		ProgramOptions expectData;
		std::string_view inputData = "--subdirectories";
		expectData.recurseSubDirectories = true;
		expectData.byteCount = true;
		expectData.lineCount = true;
		expectData.wordCount = true;

		ProgramOptions testData;
		testData.processDoubleDashOptions(inputData);
		testData.SetDefaultOptionsWhenNoFlags();
		CHECK(expectData == testData);
		if (expectData != testData)
		{
			checkWhatFailed(expectData, testData);
		}
	}

	SUBCASE("Double Dash Arguments Sub Test 3")
	{
		// -t, time execution should not affect default flags
		ProgramOptions expectData;
		std::string_view inputData = "--time-execution";
		expectData.enableExecutionTime = true;
		expectData.byteCount = true;
		expectData.lineCount = true;
		expectData.wordCount = true;

		ProgramOptions testData;
		testData.processDoubleDashOptions(inputData);
		testData.SetDefaultOptionsWhenNoFlags();
		CHECK(expectData == testData);
		if (expectData != testData)
		{
			checkWhatFailed(expectData, testData);
		}
	}
}

TEST_CASE("Test H Program Options: Process Arguments")
{
	SUBCASE("Process Argument Sub Test 1")
	{
		// When a single output flag is set, only that output should be expected
		ProgramOptions expectData;
		std::string_view inputData = "-L";
		expectData.maxLineWidth = true;
		expectData.useDefaultFlags = false;

		ProgramOptions testData;
		testData.processArgument(inputData);
		testData.SetDefaultOptionsWhenNoFlags();
		CHECK(expectData == testData);
		if (expectData != testData)
		{
			checkWhatFailed(expectData, testData);
		}
	}

	SUBCASE("Process Argument Sub Test 2")
	{
		// -R, recursive file search should not affect default flags
		ProgramOptions expectData;
		std::string_view inputData = "-R";
		expectData.recurseSubDirectories = true;
		expectData.byteCount = true;
		expectData.lineCount = true;
		expectData.wordCount = true;

		ProgramOptions testData;
		testData.processArgument(inputData);
		testData.SetDefaultOptionsWhenNoFlags();
		CHECK(expectData == testData);
		if (expectData != testData)
		{
			checkWhatFailed(expectData, testData);
		}
	}

	SUBCASE("Process Argument Sub Test 3")
	{
		// -t, time execution should not affect default flags
		ProgramOptions expectData;
		std::string_view inputData = "-t";
		expectData.enableExecutionTime = true;
		expectData.byteCount = true;
		expectData.lineCount = true;
		expectData.wordCount = true;

		ProgramOptions testData;
		testData.processArgument(inputData);
		testData.SetDefaultOptionsWhenNoFlags();
		CHECK(expectData == testData);
		if (expectData != testData)
		{
			checkWhatFailed(expectData, testData);
		}
	}

	SUBCASE("Process Argument Sub Test 4: Multiple Flags")
	{
		ProgramOptions expectData;
		std::string_view inputData = "-RtLlc";
		expectData.enableExecutionTime = true;
		expectData.recurseSubDirectories = true;
		expectData.byteCount = true;
		expectData.lineCount = true;
		expectData.maxLineWidth = true;
		expectData.useDefaultFlags = false;

		ProgramOptions testData;
		testData.processArgument(inputData);
		testData.SetDefaultOptionsWhenNoFlags();
		CHECK(expectData == testData);
		if (expectData != testData)
		{
			checkWhatFailed(expectData, testData);
		}
	}

	SUBCASE("Process Argument Sub Test 5")
	{
		// When a single output flag is set, only that output should be expected
		ProgramOptions expectData;
		std::string_view inputData = "--bytes";
		expectData.byteCount = true;
		expectData.useDefaultFlags = false;

		ProgramOptions testData;
		testData.processArgument(inputData);
		testData.SetDefaultOptionsWhenNoFlags();
		CHECK(expectData == testData);
		if (expectData != testData)
		{
			checkWhatFailed(expectData, testData);
		}
	}

	SUBCASE("Process Argument Sub Test 6")
	{
		// -R, recursive file search should not affect default flags
		ProgramOptions expectData;
		std::string_view inputData = "--subdirectories";
		expectData.recurseSubDirectories = true;
		expectData.byteCount = true;
		expectData.lineCount = true;
		expectData.wordCount = true;

		ProgramOptions testData;
		testData.processArgument(inputData);
		testData.SetDefaultOptionsWhenNoFlags();
		CHECK(expectData == testData);
		if (expectData != testData)
		{
			checkWhatFailed(expectData, testData);
		}
	}

	SUBCASE("Process Arguments Sub Test 7")
	{
		// -t, time execution should not affect default flags
		ProgramOptions expectData;
		std::string_view inputData = "--time-execution";
		expectData.enableExecutionTime = true;
		expectData.byteCount = true;
		expectData.lineCount = true;
		expectData.wordCount = true;

		ProgramOptions testData;
		testData.processArgument(inputData);
		testData.SetDefaultOptionsWhenNoFlags();
		CHECK(expectData == testData);
		if (expectData != testData)
		{
			checkWhatFailed(expectData, testData);
		}
	}
}