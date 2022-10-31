#include <memory>
#include <string>
#include "CharBuffer.h"

CharBuffer::CharBuffer()
	: capacity{ INPUTBUFFERSIZE }, actualSize{ 0 }, currentChar { &internalBuffer[0] }, lastInBuffer{ currentChar}
{
	internalBuffer = new char[INPUTBUFFERSIZE];
}

CharBuffer::CharBuffer(size_t bufferSize)
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

[[nodiscard]] bool CharBuffer::addLine(const char* line) noexcept
{
	bool canAddLine = true;
	size_t length = strlen(line);
	if (length < (capacity - actualSize))
	{
		const char* c = line;
		while (*c)
		{
			currentChar++;
			*currentChar = *c;
			*c++;
		}
		actualSize += length;
	}
	else
	{
		canAddLine = false;
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