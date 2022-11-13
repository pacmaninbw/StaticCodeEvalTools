#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <sstream>
#include "FileParser.h"
#include "FileStatistics.h"

FileParser::FileParser(FileStatistics& fileStats)
	: fileStatistics{ fileStats }
{
}

void FileParser::ParseBuffer(std::string inputBuffer) noexcept
{
	fileStatistics.setCharCount(inputBuffer.length());

	size_t lineCount = std::count(inputBuffer.begin(), inputBuffer.end(), '\n');
	fileStatistics.setToLineCount(lineCount);
	countWordsAndWhiteSpace(inputBuffer);

#if 0
	std::string::iterator currentChar = inputBuffer.begin();
	while (currentChar != inputBuffer.end())
	{
		std::string line = getCurrentLine(currentChar, inputBuffer.end());
		lineWidth(line);
		// fileStatistics.updateWidestLine(line.length());
//		parseLine(line);
	}
#endif
}

void FileParser::countWordsAndWhiteSpace(std::string& inputBuffer) noexcept
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

std::string FileParser::getCurrentLine(std::string::iterator& currentChar,
	std::string::iterator end) noexcept
{
	std::string::iterator endOfLine = std::find(currentChar, end, '\n');
	if (endOfLine != end)
	{
		endOfLine++;
	}

	std::string line(currentChar, endOfLine);
	currentChar = endOfLine;

	return line;
}

void FileParser::lineWidth(std::string line) noexcept
{
	char testWidth[1024];
	memset(testWidth, 0, 1024);
	strcpy(testWidth, line.c_str());
	size_t width = strlen(testWidth);
	fileStatistics.updateWidestLine(width);
}
