#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <fstream>
#include <iostream>
#include <sstream>
#include "Executionctrlvalues.h"
#include "FileProcessor.h"
#include "FileStatistics.h"
#include "ProgramOptions.h"
#include "ReportWriter.h"
#include "StatisticsCollector.h"
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

/*
 * For this function as far as unit test coverage goes, this is all tested
 * in the StatisticsCollectorTest unit test.
 */
static void processLoop(std::ifstream& inStream,
	FileStatistics& statistics) noexcept
{
	StatisticsCollector fileAnalyzer(statistics);

	std::stringstream inputBuffer;
	inputBuffer << inStream.rdbuf();
	fileAnalyzer.analyzeBuffer(inputBuffer.str());
}

/*
 * Processing a file includes reading the file, analyzing the input to collect
 * the statistics and then printing the statistics.
 */
static std::string processFile(const ProgramOptions& options, std::string fileName,
	FileStatistics& totalStats)
{
	std::string fileResults;

	try {
		if (fileName.empty())
		{
			std::string eMsg(
				"Programmer Error: File name is empty in "
				"FileProcessor Constructor!");
			std::runtime_error programmerError(eMsg);
			throw programmerError;
		}

		std::ifstream inStream(fileName);
		if (!inStream.is_open())
		{
			std::string eMsg("Runtime error:  Can't open file " + fileName +
				" for input.");
			std::runtime_error FileInputError(eMsg);
			throw FileInputError;
		}

		FileStatistics statistics(fileName);
		processLoop(inStream, statistics);

		inStream.close();

		ReportWriter ReportFileStatistics(options);
		fileResults = ReportFileStatistics.getResultText(statistics);
		statistics.addTotals(totalStats);
	}

	catch (const std::exception& ex)
	{
		std::cerr <<
			"Error: unable to complete processing file statistics for "
			<< fileName << " Error: " << ex.what() << std::endl;
		fileResults.clear();
	}

	return fileResults;
}

#ifndef DOCTEST_CONFIG_DISABLE
#include "../unitTest/unitTest.h"

TEST_CASE("Test M File Processor: Test processFile")
{
	ProgramOptions testOptions;
	testOptions.SetDefaultOptionsWhenNoFlags();
	SUBCASE("Sub Test 1: Test Empty File Name")
	{
		FileStatistics testTotals;
		std::string resultString = processFile(testOptions, "", testTotals);
		// If process file threw an exception then there should be no results
		// to report
		CHECK(resultString.empty() == true);
		CHECK(testTotals.areResultsEmpty() == true);
	}

	SUBCASE("Sub Test 2: Test File That Doesn't Exist")
	{
		FileStatistics testTotals;
		std::string resultString = processFile(testOptions, "notAFile.txt", testTotals);
		// If process file threw an exception then there should be no results
		// to report
		CHECK(resultString.empty() == true);
		CHECK(testTotals.areResultsEmpty() == true);
	}

	SUBCASE("Sub Test 3: Test Existing File")
	{
		std::string realFileSpec = FindUnitTestDirectorRoot();
		CHECK(realFileSpec.empty() == false);
		realFileSpec += "/unitTest.h";
		FileStatistics testTotals;
		std::string resultString = processFile(testOptions, realFileSpec, testTotals);
		CHECK(resultString.empty() == false);
		CHECK(testTotals.areResultsEmpty() == false);
	}
}
#endif

std::string processAllFiles(const ExecutionCtrlValues& executionControl)
{
	ReportWriter TotalsReporter(executionControl.options);
	FileStatistics allFiles("Totals");

	std::string resultsToDisplay(TotalsReporter.getColumnHeadingAsOneString());

	for (const auto& currentFile : executionControl.filesToProcess)
	{
		try
		{
			std::string fileResults = processFile(executionControl.options,
				currentFile, allFiles);
			if (!fileResults.empty())
			{
				resultsToDisplay += fileResults;
			}
		}
		catch (const std::runtime_error& re)
		{
			std::cerr << re.what() << "\n";
		}
	}

	resultsToDisplay += TotalsReporter.getColumnHeadingAsOneString();
	resultsToDisplay += TotalsReporter.getResultText(allFiles);

	return resultsToDisplay;
}
