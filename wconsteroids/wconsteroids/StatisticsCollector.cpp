#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include "StatisticsCollector.h"
#include "FileStatistics.h"

static constexpr std::size_t tabSize = 8;

StatisticsCollector::StatisticsCollector(FileStatistics& fileStats)
	: fileStatistics{ fileStats }
{
}

void StatisticsCollector::analyzeBuffer(std::string_view inputBuffer) noexcept
{
	auto endBuffer = inputBuffer.end();
	auto startBuffer = inputBuffer.begin();

	std::uintmax_t bufferSize = std::filesystem::file_size(fileStatistics.getFileName());
	fileStatistics.setCharCount(bufferSize);

	std::size_t lineCount = std::count(startBuffer, endBuffer, '\n');
	fileStatistics.setToLineCount(lineCount);
	countWordsAndWhiteSpace(inputBuffer);

	auto currentChar = startBuffer;
	while (currentChar != endBuffer)
	{
		updateWidestLine(currentChar, endBuffer);
	}
}

void StatisticsCollector::countWordsAndWhiteSpace(std::string_view inputBuffer) noexcept
{
	std::size_t wordCount = 0;
	std::size_t whiteSpaceCount = 0;
	auto currentChar = inputBuffer.begin();
	auto endOfInput = inputBuffer.end();
	bool inWord = false;
	for ( ; currentChar != endOfInput; )
	{
            while (std::isspace(static_cast<unsigned char>(*currentChar)))
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

void StatisticsCollector::updateWidestLine(std::string_view::const_iterator& currentChar,
                                           std::string_view::const_iterator end) noexcept
{
	auto endOfLine = std::find(currentChar, end, '\n');
	if (endOfLine != end)
	{
		endOfLine++;
	}
	std::size_t lineWidth = endOfLine - currentChar;

	// See https://github.com/coreutils/coreutils/blob/master/src/wc.c to
	// observe how tabs are counted.
	std::size_t tabCount = std::count(currentChar, endOfLine, '\t');
	lineWidth += tabCount * tabSize;


	fileStatistics.updateWidestLine(lineWidth);

	std::string line(currentChar, endOfLine);
	currentChar = endOfLine;
}
