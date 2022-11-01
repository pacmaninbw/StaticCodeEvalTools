#ifndef CHARBUFFER_H
#define CHARBUFFER_H
#define INPUTBUFFERSIZE	(2048 * 4)

#include <string>

/*
 * Buffer for character file input. 
 * Maintains a pointer to the last character in the current block of input.
 * Returns the current character
 * Returns a line in the block.
 * Follows rule of 5 since Move might be needed.
 */
class CharBuffer
{
public:
	CharBuffer(size_t buffSize);
	CharBuffer(CharBuffer&& other) noexcept;			// Move Constructor
	CharBuffer& operator=(CharBuffer&& other) noexcept;	// Move Assignment
	CharBuffer& operator=(const CharBuffer& other);		// Copy Assignment
	CharBuffer(const CharBuffer& other);				// Copy Constructor
	~CharBuffer();
	// returns a NULL terminated array of characters
	[[nodiscard]] char* getCurrentLine() noexcept;
	// The addLine function returns true if the line can be added and
	// false if the buffer does not have the capacity to store the new line
	[[nodiscard]] bool addLine(std::string& line) noexcept;
	// While the inline key word is only a recommendation, hopefully these 2 functions
	// can be inline.
	inline char getCurrentCharacter() const noexcept { return *currentChar; }
	inline void addCharacter(char c) noexcept { internalBuffer[actualSize] = c; actualSize++; lastInBuffer++; };
	inline void inputComplete() noexcept { lastInBuffer = &internalBuffer[actualSize]; currentChar = &internalBuffer[0]; }
	bool endOfBuffer() noexcept { return currentChar == lastInBuffer; }

private:
	char* internalBuffer;
	char* currentChar;
	char* lastInBuffer;
	size_t capacity;
	size_t actualSize;
};
#endif // CHARBUFFER_H
