#include <iostream>
#include <fstream>
#include <string>
#include "CharBuffer.h"
#include "FileReader.h"

static std::ifstream inputFile;

FileReader::FileReader(std::string inFileName)
	:endOfFileEncountered{false}, charCount{0}, lineCount{0}
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
}

FileReader::~FileReader()
{
	inputFile.close();
}

CharBuffer* FileReader::readBlockOfText()
{
	CharBuffer* inputBuffer = new CharBuffer(CB_INPUTBUFFERSIZE);
	bool bufferFull = false;
	std::string line;

	do {
		getline(inputFile, line);
		if (line.size() > 0)
		{
			bufferFull = !inputBuffer->addLine(line);
			// These statistics are collected here for performance reasons.
			// The method of input allows for fast collection of the data.
			lineCount++;
			charCount += line.size();
		}

		endOfFileEncountered = inputFile.eof();
	} while (!bufferFull && !endOfFileEncountered);

	return inputBuffer;
}

bool FileReader::atEndOfFile()
{
	return (endOfFileEncountered || inputFile.peek() == EOF);
}
