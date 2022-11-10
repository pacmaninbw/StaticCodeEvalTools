#include <cctype>
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
	std::string::iterator currentChar = inputBuffer.begin();
	while (currentChar != inputBuffer.end())
	{
		std::string line = getCurrentLine(currentChar, inputBuffer.end());
		fileStatistics.addToCharCount(line.size());
		fileStatistics.updateWidestLine(line.size());
		fileStatistics.incrementTotalLines();
		parseLine(line);
	}
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

