#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <string>
#include <vector>
#include "CharBuffer.h"
#include "FileStatistics.h"

class FileParser
{
public:
	FileParser(FileStatistics& fileStats);
	void ParseBuffer(CharBuffer& inputbuffer) noexcept;
	void addBufferStats(FileStatistics& processorStats) noexcept
	{
		fileStatistics.addTotals(processorStats);
	}

protected:
	void parseLine(std::vector<unsigned char> line) noexcept;
	std::vector<std::string> tokenize(std::vector<unsigned char> line) noexcept;

private:
	FileStatistics& fileStatistics;
	bool inComment;
	bool inBlockComment;
};

#endif // FILEPARSER_H