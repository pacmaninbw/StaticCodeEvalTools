#include <iostream>
#include <fstream>
#include <sstream>
#include "FileParser.h"
#include "FileProcessor.h"
#include "FileStatistics.h"
#include "ReportWriter.h"

static constexpr size_t InputBufferSize = 8 * 1024;

FileProcessor::FileProcessor(std::string inFileName, ProgramOptions& progOptions)
	: options{ progOptions }
{
	if (inFileName.empty())
	{
		std::string eMsg(
			"Programmer Error: File name is empty in "
			"FileProcessor Constructor!");
		std::runtime_error programmerError(eMsg);
		throw programmerError;
	}
	fileName = inFileName;
	statistics.setFileName(fileName);

}


void FileProcessor::mergeStatistics(FileStatistics& allFileStats)
{
	statistics.addTotals(allFileStats);
}

FileStatistics FileProcessor::getStatistics()
{
	return statistics;
}

/*
 * Processing a file includes reading the file, parsing the input to collect
 * the statistics and then pringing the statistics.
 */
bool FileProcessor::processFile()
{
	bool processComplete = true;

	try{
		std::ifstream inStream(fileName);
		if (!inStream.is_open())
		{
			std::string eMsg("Runtime error:  Can't open file " + fileName +
				" for input.");
			std::runtime_error FileInputError(eMsg);
			throw FileInputError;
		}

		processLoop(inStream);

		ReportWriter ReportFileStatistics(options);
		ReportFileStatistics.printResult(statistics);
	}

	catch(std::exception ex)
	{
		std::cerr <<
			"Error: unable to complete processing file statistics for "
			<< fileName << " Error: " << ex.what() << std::endl;
		processComplete = false;
	}

	return processComplete;
}

void FileProcessor::processLoop(std::ifstream& inStream) noexcept
{
	FileParser fileParser(statistics);

	do
	{
		std::stringstream inoutBuffer;
		inoutBuffer << inStream.rdbuf();
		fileParser.ParseBuffer(inoutBuffer.str());
		fileParser.addBufferStats(statistics);
	} while (!inStream.eof());
}

