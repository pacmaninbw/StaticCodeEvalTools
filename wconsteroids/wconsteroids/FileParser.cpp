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
	: fileStatistics{ fileStats },
	inComment{ false },
	inBlockComment{ false }
{
}

void FileParser::ParseBuffer(std::string inputBuffer) noexcept
{
	fileStatistics.setCharCount(inputBuffer.length());

//	terminateFileWithNewLine(inputBuffer);

	size_t lineCount = std::count(inputBuffer.begin(), inputBuffer.end(), '\n');
	fileStatistics.setToLineCount(lineCount);
	countWords(inputBuffer);

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

void FileParser::parseLine(std::string line) noexcept
{
	size_t whiteSpaceCount = 0;
	std::vector<std::string> tokens = tokenize(line);
	fileStatistics.addToWordCount(tokens.size());
}

static std::string tokenizeWord(std::string::iterator& currentChar)
{
	std::string token;

	while (isalnum(*currentChar))
	{
		token.push_back(*currentChar++);
	}

	return token;
}

static std::string tokenizeOperator(std::string::iterator& currentChar)
{
	std::string token;

	while ((!isalnum(*currentChar)) && (!isspace(*currentChar)))
	{
		token.push_back(*currentChar++);
	}

	return token;
}

void FileParser::countWords(std::string& inputBuffer) noexcept
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

std::vector<std::string> FileParser::tokenize(std::string line) noexcept
{
	std::vector<std::string> tokens;
	std::string::iterator currentChar;
	size_t whiteSpaceCount = 0;

	for (currentChar = line.begin(); *currentChar != '\n' &&
		currentChar != line.end(); )
	{
		while (*currentChar != '\n' && currentChar != line.end() &&
			isspace(*currentChar))
		{
			currentChar++;
			whiteSpaceCount++;
		}

		if (currentChar != line.end() && *currentChar != '\n')
		{
			std::string token = (isalnum(*currentChar)) ? tokenizeWord(currentChar):
				tokenizeOperator(currentChar);
			if (!token.empty())
			{
				tokens.push_back(token);
			}
		}
	}

	fileStatistics.addToWhitespace(whiteSpaceCount);

	return tokens;
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

void FileParser::terminateFileWithNewLine(std::string& inputBuffer) noexcept
{
	// Correct a bug in the tokenizer when the file isn't terminated with a new line
	std::string::iterator lastChar = inputBuffer.end();
	--lastChar;
	if (*lastChar != '\n')
	{
		inputBuffer.push_back('\n');
	}
}

void FileParser::lineWidth(std::string line) noexcept
{
	char testWidth[1024];
	memset(testWidth, 0, 1024);
	strcpy(testWidth, line.c_str());
	size_t width = strlen(testWidth);
	fileStatistics.updateWidestLine(width);
}

