#include <iostream>
#include <fstream>
#include <string>
#include "CharBuffer.h"
#include "FileReader.h"

static std::ifstream inputFile;

FileReader::FileReader(std::string inFileName)
	:endOfFileEncountered{false}
{
	fileName = inFileName;
	inputFile.open(fileName);
	if (!inputFile.is_open())
	{
		std::string eMsg("Runtime error:  Can't open file " + fileName + " for input.");
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

	inputBuffer->inputComplete();

	return inputBuffer;
}

bool FileReader::atEndOfFile()
{
	return (endOfFileEncountered || inputFile.peek() == EOF);
}
