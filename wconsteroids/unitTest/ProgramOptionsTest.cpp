#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>
#include "ProgramOptions.h"
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

TEST_CASE("Program Options Unit Test 1: Constructor")
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
	CHECK(testConstructor.isDefaultFlagsSet() == true);
}