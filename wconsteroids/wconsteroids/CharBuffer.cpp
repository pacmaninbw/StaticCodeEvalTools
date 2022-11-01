#include <memory>
#include <string>
#include "CharBuffer.h"

/*
 * Rule of 5 Constructor 
 */
CharBuffer::CharBuffer(size_t bufferSize = INPUTBUFFERSIZE)
	: capacity{ bufferSize }, actualSize{ 0 }, currentChar{ &internalBuffer[0] }, lastInBuffer{ currentChar }
{
	internalBuffer = new char[capacity];
	memset(internalBuffer, 0, capacity);
}

/*
 * Rule of 5 Implement move semantics
 */
CharBuffer::CharBuffer(CharBuffer&& other) noexcept
	: capacity{ other.capacity }, actualSize{ other.actualSize },
	internalBuffer{other.internalBuffer},
	currentChar{ &internalBuffer[0] }, lastInBuffer{ currentChar }
{

}

/*
 * Rule of 5 Implement move operator
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

/*
 * Rule of 5 Copy Constructor
 */
CharBuffer::CharBuffer(const CharBuffer& original)
	: capacity{ original.capacity }, actualSize{original.actualSize}
{
	internalBuffer = new char[capacity];
	memset(internalBuffer, 0, capacity);
	memcpy(internalBuffer, original.internalBuffer, actualSize);
	currentChar = internalBuffer;
	lastInBuffer = &internalBuffer[actualSize];
}
/*
 * Rule of 5 Copy Operator
 */
CharBuffer& CharBuffer::operator=(const CharBuffer& original)
{
	capacity = original.capacity;
	actualSize = original.actualSize;
	internalBuffer = new char[capacity];
	memset(internalBuffer, 0, capacity);
	memcpy(internalBuffer, original.internalBuffer, actualSize);
	currentChar = internalBuffer;
	lastInBuffer = &internalBuffer[actualSize];

	return *this;
}

/*
 * Rule of 5 Destructor 
 */
CharBuffer::~CharBuffer()
{
	delete[] internalBuffer;
	capacity = 0;
	actualSize = 0;
	currentChar = nullptr;
	lastInBuffer = nullptr;
}

/*
 * Add a line of text to the buffer.
 * Return value indicates if there was sufficient space to store the new line.
 */
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

/*
 * Return a copy of the current line and advance the current pointer to the new line.
 * Caller is responsible for deleting the char array after use.
 */
[[nodiscard]] char* CharBuffer::getCurrentLine() noexcept
{
	if (!*currentChar)
	{
		return nullptr;
	}

	// Advance to the start of the line
	if (*currentChar == '\n')
	{
		currentChar++;
	}

	char* findEndOfLine = currentChar;
	for (; *findEndOfLine != '\n' && findEndOfLine < lastInBuffer; findEndOfLine++);

	size_t length = findEndOfLine - currentChar;

	char* line = new char[length+1];
	memset(line, 0, length + 1);
	memcpy(line, currentChar, length);

	currentChar = findEndOfLine + 1;	// reset currentChar to point to the new start location.
	if (currentChar > lastInBuffer)
	{
		currentChar = lastInBuffer;
	}


	return line;
}
