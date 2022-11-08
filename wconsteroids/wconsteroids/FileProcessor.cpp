#include <iostream>
#include "FileParser.h"
#include "FileProcessor.h"
#include "FileReader.h"
#include "FileStatistics.h"

FileProcessor::FileProcessor(std::string inFileName)
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

bool FileProcessor::processFile()
{
	bool processComplete = true;
	FileParser fileParser(statistics);

	try{
		do
		{
			// 8K buffer should be fine.
			CharBuffer inputBuffer(8192);
			reader.readBlockOfText(inputBuffer);
			inputBuffer.resetCurrentCharacter();
			fileParser.ParseBuffer(inputBuffer);
			fileParser.addBufferStats(statistics);
		} while (!reader.atEndOfFile());
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