#include <vector>
#include "CharBuffer.h"
#include "FileParser.h"
#include "FileStatistics.h"

FileParser::FileParser(FileStatistics& fileStats)
	: fileStatistics{fileStats}
{
}

void FileParser::ParseBuffer(CharBuffer& inputBuffer)
{
	while (!inputBuffer.endOfBuffer())
	{
		std::vector<char> line = inputBuffer.getCurrentLine();
		fileStatistics.addToCharCount(line.size());
		fileStatistics.updateWidestLine(line.size());
		fileStatistics.incrementTotalLines();
	}
}
