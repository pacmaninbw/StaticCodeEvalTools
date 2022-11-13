#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <iterator>
#include <string>
#include <vector>
#include "FileStatistics.h"

class FileParser
{
public:
	FileParser(FileStatistics& fileStats);
	void ParseBuffer(std::string inputbuffer) noexcept;
	void addBufferStats(FileStatistics& processorStats) noexcept
	{
		fileStatistics.addTotals(processorStats);
	}

protected:
	std::string getCurrentLine(std::string::iterator& currentChar,
		std::string::iterator end) noexcept;
	void lineWidth(std::string line) noexcept;
	void countWordsAndWhiteSpace(std::string& inputBuffer) noexcept;

private:
	FileStatistics& fileStatistics;
};

#endif // FILEPARSER_H