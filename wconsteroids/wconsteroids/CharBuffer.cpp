#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include "CharBuffer.h"

CharBuffer::CharBuffer(size_t bufferSize)
	: capacity{ bufferSize }, actualSize{ 0 }, currentCharIdx{ 0 }
{
	internalBuffer.reserve(capacity);
}

/*
 * Add a line of text to the buffer. Return value indicates if there
 * was sufficient space to store the new line.
 */
[[nodiscard]] bool CharBuffer::addLine(std::string& line) noexcept
{
	bool canAddLine = (line.size() < (capacity - actualSize));

	if (canAddLine)
	{
		std::copy(line.begin(), line.end(),
			std::back_inserter(internalBuffer));
		actualSize += line.size();
	}

	return canAddLine;
}

/*
 * Return a copy of the current line and advance the current
 * pointer to the new line.
 */
std::vector<unsigned char> CharBuffer::getCurrentLine() noexcept
{
	std::vector<unsigned char> line;

	for ( ; internalBuffer[currentCharIdx] != '\n' &&
		currentCharIdx < actualSize; currentCharIdx++)
	{
		line.push_back(internalBuffer[currentCharIdx]);
	}

	// include the newline in the return value and advance the current
	// character index
	line.push_back(internalBuffer[currentCharIdx]);
	currentCharIdx++;

	return line;
}
