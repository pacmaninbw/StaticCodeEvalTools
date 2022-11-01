#include <iostream>
#include <fstream>
#include <string>
#include "CharBuffer.h"
#include "FileReader.h"

static std::ifstream inputFile;

FileReader::FileReader(std::string inFileName)
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
	CharBuffer* inputBuffer = new CharBuffer(INPUTBUFFERSIZE);

	return inputBuffer;
}
