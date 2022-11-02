#include <iostream>
#include "FileProcessor.h"
#include "FileReader.h"
#include "FileStatistics.h"

FileProcessor::FileProcessor(std::string inFileName)
{
	if (fileName.size() < 1)
	{
		std::string eMsg("Programmer Error: File name is empty in FileProcessor Constructor!");
		std::runtime_error programmerError(eMsg);
		throw programmerError;
	}
	fileName = inFileName;
	statistics.setFileName(fileName);
	reader = new FileReader(fileName);
}

FileProcessor::~FileProcessor()
{
	delete reader;
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

	while (!reader->atEndOfFile())
	{
		CharBuffer* inputBuffer = reader->readBlockOfText();
		statistics.addToCharCount(reader->getCharCount());
		statistics.addToLineCount(reader->getLineCount());
	}

	return processComplete;
}