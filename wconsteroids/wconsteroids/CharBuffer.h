#ifndef CHARBUFFER_H
#define CHARBUFFER_H

/*
 * Old style C macros so that symbolic constants are not defined in multiple files.
 * Attempting to make it type safe with the static cast to size_t. The multiplier
 * is used to be able to increase or decrease the size of the buffer easily.
 * CB_ stands for CharBuffer.
 */
#define CB_MINBUFFERSIZE				2048	// 2 Kbytes
#define CB_PERFORMANCE_MULTIPLIER		4
#define CB_INPUTBUFFERSIZE	static_cast<size_t>(CB_MINBUFFERSIZE * CB_PERFORMANCE_MULTIPLIER)

#include <string>
#include <vector>

/*
 * Buffer for character file input. 
 * Maintains a pointer to the last character in the current block of input.
 * Returns the current character
 * Returns a line in the block.
 * 
 * Follows rule of 5 since Move might be needed. Implementations that use this
 * buffer might be multi-threaded and move semantics would be necessary for
 * performance reasons. One thread would be the process that reads a file and
 * creates the buffer, a second process would be the consumer of the buffer.
 */
class CharBuffer
{
public:
	CharBuffer(size_t bufferSize);
	CharBuffer(CharBuffer&& other) noexcept;			// Move Constructor
	CharBuffer& operator=(CharBuffer&& other) noexcept;	// Move Assignment
	CharBuffer& operator=(const CharBuffer& original);	// Copy Assignment
	CharBuffer(const CharBuffer& original);				// Copy Constructor
	~CharBuffer();
	std::vector<char> getCurrentLine() noexcept;
	// The addLine function returns true if the line can be added and
	// false if the buffer does not have the capacity to store the new line
	[[nodiscard]] bool addLine(std::string& line) noexcept;
	// While the inline key word is only a recommendation, hopefully these 2 functions
	// can be inline.
	inline char getCurrentCharacter() const noexcept { return internalBuffer[currentCharIdx]; }
	inline void addCharacter(char c) noexcept { internalBuffer[actualSize] = c; actualSize++; };
	inline void inputComplete() noexcept { currentCharIdx = 0; }
	bool endOfBuffer() noexcept { return currentCharIdx >= actualSize; }

private:
	std::vector<char> internalBuffer;
	size_t currentCharIdx;
	size_t capacity;
	size_t actualSize;
};
#endif // CHARBUFFER_H
