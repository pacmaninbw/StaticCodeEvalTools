#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "CharBuffer.h"
#include "FileStatistics.h"

class FileParser
{
	FileParser();
	void ParseBuffer(CharBuffer* inputbuffer);
};

#endif // FILEPARSER_H