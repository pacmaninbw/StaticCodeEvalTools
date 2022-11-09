#include <cctype>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "CharBuffer.h"
#include "FileParser.h"
#include "FileStatistics.h"

FileParser::FileParser(FileStatistics& fileStats)
	: fileStatistics{ fileStats },
	inComment{ false },
	inBlockComment{ false }
{
}

void FileParser::ParseBuffer(CharBuffer& inputBuffer) noexcept
{
	while (!inputBuffer.endOfBuffer())
	{
		std::vector<unsigned char> line = inputBuffer.getCurrentLine();
		fileStatistics.addToCharCount(line.size());
		fileStatistics.updateWidestLine(line.size());
		fileStatistics.incrementTotalLines();
		parseLine(line);
	}
}

void FileParser::parseLine(std::vector<unsigned char> line) noexcept
{
	size_t whiteSpaceCount = 0;
	std::vector<std::string> tokens = tokenize(line);
	fileStatistics.addToWordCount(tokens.size());
}

static std::string tokenizeWord(std::vector<unsigned char>::iterator& currentChar)
{
	std::string token;

	while (isalnum(*currentChar))
	{
		token.push_back(*currentChar++);
	}

	return token;
}

static std::string tokenizeOperator(std::vector<unsigned char>::iterator& currentChar)
{
	std::string token;

	while ((!isalnum(*currentChar)) && (!isspace(*currentChar)))
	{
		token.push_back(*currentChar++);
	}

	return token;
}

std::vector<std::string> FileParser::tokenize(std::vector<unsigned char> line) noexcept
{
	std::vector<std::string> tokens;
	std::vector<unsigned char>::iterator currentChar;
	size_t whiteSpaceCount = 0;
	std::string sline(line.begin(), line.end());

	for (currentChar = line.begin(); *currentChar != '\n'; )
	{
		while (*currentChar != '\n' && isspace(*currentChar))
		{
			currentChar++;
			whiteSpaceCount++;
		}

		if (*currentChar != '\n')
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
