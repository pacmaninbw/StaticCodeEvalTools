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
	void parseLine(std::string line) noexcept;
	std::vector<std::string> tokenize(std::string line) noexcept;
	std::string getCurrentLine(std::string::iterator& currentChar,
		std::string::iterator end) noexcept;
	void terminateFileWithNewLine(std::string& inputBuffer) noexcept;
	void lineWidth(std::string line) noexcept;
	void countWords(std::string& inputBuffer) noexcept;

private:
	FileStatistics& fileStatistics;
	bool inComment;
	bool inBlockComment;
};

#endif // FILEPARSER_H