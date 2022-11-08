#include <iostream>
#include <memory>
#include "CharBuffer.h"
#include "FileParser.h"
#include "FileProcessor.h"
#include "FileReader.h"
#include "FileStatistics.h"
#include "ReportWriter.h"

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
	reader.setFileName(fileName);
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
	constexpr size_t InputBufferSize = 8192;
	bool processComplete = true;
	FileParser fileParser(statistics);

	try{
		do
		{
			CharBuffer inputBuffer(InputBufferSize);
			reader.readBlockOfText(inputBuffer);
			fileParser.ParseBuffer(inputBuffer);
			fileParser.addBufferStats(statistics);
		} while (!reader.atEndOfFile());

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