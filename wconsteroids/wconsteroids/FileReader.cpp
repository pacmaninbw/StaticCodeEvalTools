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
