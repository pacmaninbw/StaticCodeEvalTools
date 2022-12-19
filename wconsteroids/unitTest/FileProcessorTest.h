#ifndef FILE_PROCESSOR_UNIT_TEST_H
#define FILE_PROCESSOR_UNIT_TEST_H

/*
 * The code in this file tests static functions in FileProcessor.cpp, so
 * it needs to be included by the source file.
 */
#include "../unitTest/unitTest.h"

TEST_CASE("E1 File Processor: Test processFile")
{
	ProgramOptions testOptions;
	testOptions.SetDefaultOptionsWhenNoFlags();
	SUBCASE("E.1: Test Empty File Name")
	{
		FileStatistics testTotals;
		// This test does generate an error message in the outpuit.
		std::string resultString = processFile(testOptions, "", testTotals);
		// If process file threw an exception then there should be no results
		// to report
		CHECK(resultString.empty() == true);
		CHECK(testTotals.areResultsEmpty() == true);
	}

	SUBCASE("E1.2: Test File That Doesn't Exist")
	{
		FileStatistics testTotals;
		// This test does generate an error message in the outpuit.
		std::string resultString = processFile(testOptions, "notAFile.txt", testTotals);
		// If process file threw an exception then there should be no results
		// to report
		CHECK(resultString.empty() == true);
		CHECK(testTotals.areResultsEmpty() == true);
	}

	SUBCASE("E1.3: Test Existing File")
	{
		std::string realFileSpec = FindUnitTestDirectorRoot();
		CHECK(realFileSpec.empty() == false);
		realFileSpec += "/smallerStatTest.txt";
		FileStatistics testTotals;
		std::string resultString = processFile(testOptions, realFileSpec, testTotals);
		CHECK(resultString.empty() == false);
		CHECK(testTotals.areResultsEmpty() == false);
	}
}

#endif // FILE_PROCESSOR_UNIT_TEST_H
