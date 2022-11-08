#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "CharBuffer.h"
#include "FileStatistics.h"

class FileParser
{
public:
	FileParser(FileStatistics& fileStats);
	void ParseBuffer(CharBuffer& inputbuffer);
	void addBufferStats(FileStatistics& processorStats)
	{
		fileStatistics.addTotals(processorStats);
	}

private:
	FileStatistics& fileStatistics;
};

#endif // FILEPARSER_H