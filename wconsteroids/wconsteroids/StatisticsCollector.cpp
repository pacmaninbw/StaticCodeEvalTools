#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include "StatisticsCollector.h"
#include "FileStatistics.h"

static constexpr size_t tabSize = 8;

StatisticsCollector::StatisticsCollector(FileStatistics& fileStats)
	: fileStatistics{ fileStats }
{
}

void StatisticsCollector::analyzeBuffer(std::string inputBuffer) noexcept
{
	std::string::iterator endBuffer = inputBuffer.end();
	std::string::iterator startBuffer = inputBuffer.begin();

	std::uintmax_t bufferSize = std::filesystem::file_size(fileStatistics.getFileName());
	fileStatistics.setCharCount(bufferSize);

	size_t lineCount = std::count(startBuffer, endBuffer, '\n');
	fileStatistics.setToLineCount(lineCount);
	countWordsAndWhiteSpace(inputBuffer);

	std::string::iterator currentChar = startBuffer;
	while (currentChar != endBuffer)
	{
		updateWidestLine(currentChar, endBuffer);
	}
}

void StatisticsCollector::countWordsAndWhiteSpace(std::string& inputBuffer) noexcept
{
	size_t wordCount = 0;
	size_t whiteSpaceCount = 0;
	std::string::iterator currentChar = inputBuffer.begin();
	std::string::iterator endOfInput = inputBuffer.end();
	bool inWord = false;

	for ( ; currentChar != endOfInput; )
	{
		while (isspace(*currentChar))
		{
			whiteSpaceCount++;
			currentChar++;
			if (currentChar == endOfInput)
			{
				break;
			}
		}

		while (!(currentChar == endOfInput) && !isspace(*currentChar))
		{
			inWord = true;
			currentChar++;
			if (currentChar == endOfInput)
			{
				wordCount++;
				inWord = false;
				break;
			}
		}

		if (inWord)
		{
			wordCount++;
			inWord = false;
		}
	}

	fileStatistics.addToWhitespace(whiteSpaceCount);
	fileStatistics.setWordCount(wordCount);
}

void StatisticsCollector::updateWidestLine(std::string::iterator& currentChar,
	std::string::iterator end) noexcept
{
	std::string::iterator endOfLine = std::find(currentChar, end, '\n');
	if (endOfLine != end)
	{
		endOfLine++;
	}
	size_t lineWidth = endOfLine - currentChar;

	// See https://github.com/coreutils/coreutils/blob/master/src/wc.c to
	// observe how tabs are counted.
	size_t tabCount = std::count(currentChar, endOfLine, '\t');
	lineWidth += tabCount * tabSize;


	fileStatistics.updateWidestLine(lineWidth);

	std::string line(currentChar, endOfLine);
	currentChar = endOfLine;
}
