#include <iostream>
#include <fstream>
#include <string>
#include "CharBuffer.h"
#include "FileReader.h"

static std::ifstream inputFile;

FileReader::FileReader()
	:endOfFileEncountered{ false }, charCount{ 0 }, lineCount{ 0 },
	firstRead{ true }
{

}

FileReader::FileReader(std::string inFileName)
	:endOfFileEncountered{ false }, charCount{ 0 }, lineCount{ 0 },
	firstRead{ true }
{
	fileName = inFileName;
	inputFile.open(fileName);
	if (!inputFile.is_open())
	{
		std::string eMsg("Runtime error:  Can't open file " + fileName +
			" for input.");
		std::runtime_error FileInputError(eMsg);
		throw FileInputError;
	}
	firstRead = false;
}

FileReader::~FileReader()
{
	inputFile.close();
}

void FileReader::readBlockOfText(CharBuffer& inputBuffer)
{
	if (firstRead)
	{
		inputFile.open(fileName);
		if (!inputFile.is_open())
		{
			std::string eMsg("Runtime error:  Can't open file " + fileName +
				" for input.");
			std::runtime_error FileInputError(eMsg);
			throw FileInputError;
		}
		firstRead = false;
	}

	bool bufferFull = false;
	std::string line;

	do {
		getline(inputFile, line);
		line += "\n";	// getLine strips new lines
		bufferFull = !inputBuffer.addLine(line);
		if (!bufferFull)
		{
			// These statistics are collected here for performance reasons.
			// The method of input allows for fast collection of the data.
			lineCount++;
			charCount += (line.size() > 0) ? line.size() : 1;
		}

		endOfFileEncountered = inputFile.eof();
	} while (!bufferFull && !endOfFileEncountered);
}

bool FileReader::atEndOfFile()
{
	return (endOfFileEncountered || inputFile.peek() == EOF);
}
