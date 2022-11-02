#include <memory>
#include <iterator>
#include <string>
#include <vector>
#include "CharBuffer.h"

/*
 * Rule of 5 Constructor 
 */
CharBuffer::CharBuffer(size_t bufferSize = CB_INPUTBUFFERSIZE)
	: capacity{ bufferSize }, actualSize{ 0 }, currentCharIdx{ 0 }
{
	internalBuffer.reserve(capacity);
}

/*
 * Rule of 5 Implement move semantics
 */
CharBuffer::CharBuffer(CharBuffer&& other) noexcept
	: capacity{ other.capacity }, actualSize{ other.actualSize },
	internalBuffer{std::move(other.internalBuffer)},
	currentCharIdx{ 0 }
{

}

/*
 * Rule of 5 Implement move operator
 */
CharBuffer& CharBuffer::operator=(CharBuffer&& other) noexcept
{
	internalBuffer =  std::move(other.internalBuffer);
	capacity = other.capacity;
	actualSize = other.actualSize;
	currentCharIdx = 0;

	return *this;
}

/*
 * Rule of 5 Copy Constructor
 */
CharBuffer::CharBuffer(const CharBuffer& original)
	: capacity{ original.capacity }, actualSize{original.actualSize}
{
	internalBuffer = original.internalBuffer;
	currentCharIdx = original.currentCharIdx;
}
/*
 * Rule of 5 Copy Operator
 */
CharBuffer& CharBuffer::operator=(const CharBuffer& original)
{
	capacity = original.capacity;
	actualSize = original.actualSize;
	internalBuffer = original.internalBuffer;
	currentCharIdx = original.currentCharIdx;

	return *this;
}

/*
 * Rule of 5 Destructor 
 */
CharBuffer::~CharBuffer()
{
	capacity = 0;
	actualSize = 0;
	currentCharIdx = 0;
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
		std::copy(line.begin(), line.end(), std::back_inserter(internalBuffer));
		currentCharIdx += line.size();
		actualSize += line.size();
	}

	return canAddLine;
}

/*
 * Return a copy of the current line and advance the current pointer to the new line.
 */
std::vector<char> CharBuffer::getCurrentLine() noexcept
{
	std::vector<char> line;

	std::vector<char>::iterator findEndOfLine = std::find(internalBuffer.begin()+currentCharIdx, internalBuffer.end(), '\n');
	if (findEndOfLine == internalBuffer.end())
	{
		findEndOfLine = internalBuffer.begin() + actualSize;
	}

	std::copy(internalBuffer.begin() + currentCharIdx, findEndOfLine, line.begin());
	currentCharIdx += line.size();


	return line;
}
