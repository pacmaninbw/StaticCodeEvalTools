#include <memory>
#include <string>
#include "CharBuffer.h"

CharBuffer::CharBuffer(size_t bufferSize = INPUTBUFFERSIZE)
	: capacity{ bufferSize }, actualSize{ 0 }, currentChar{ &internalBuffer[0] }, lastInBuffer{ currentChar }
{
	internalBuffer = new char[capacity];
}

/*
 * Implement move semantics
 */
CharBuffer::CharBuffer(CharBuffer&& other) noexcept
	: capacity{ other.capacity }, actualSize{ other.actualSize },
	internalBuffer{other.internalBuffer},
	currentChar{ &internalBuffer[0] }, lastInBuffer{ currentChar }
{

}

/*
 * Implement move operator
 */
CharBuffer& CharBuffer::operator=(CharBuffer&& other) noexcept
{
	std::swap(internalBuffer, other.internalBuffer);
	capacity = other.capacity;
	actualSize = other.actualSize;
	currentChar = &internalBuffer[0];
	lastInBuffer = &internalBuffer[actualSize];

	return *this;
}

CharBuffer::~CharBuffer()
{
	delete[] internalBuffer;
	capacity = 0;
	actualSize = 0;
	currentChar = nullptr;
	lastInBuffer = nullptr;
}

[[nodiscard]] bool CharBuffer::addLine(std::string& line) noexcept
{
	bool canAddLine = line.size() > 0 && (line.size() < capacity - actualSize);

	if (canAddLine)
	{
		line.copy(currentChar, line.size());
		currentChar += line.size();
		actualSize += line.size();
	}

	return canAddLine;
}

[[nodiscard]] char* CharBuffer::getCurrentLine()
{
	if (!*currentChar)
	{
		return nullptr;
	}

	if (*currentChar == '\n')
	{
		currentChar++;
	}

	char* findEndOfLine = currentChar;
	while (*findEndOfLine != '\n' && findEndOfLine <= lastInBuffer)
	{
		findEndOfLine++;
	}

	size_t length = findEndOfLine - currentChar;

	char* line = new char[length+1];
	char* l = line;
	while (l < findEndOfLine)
	{
		*l = *currentChar;
		l++;
		currentChar++;
	}
	line[length] = 0;

	return line;
}
