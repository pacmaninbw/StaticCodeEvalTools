#ifndef CHARBUFFER_H
#define CHARBUFFER_H

/*
 * Buffer for character file input.
 * Maintains an index to the last character in the current block of input.
 * Returns the current character
 * Returns a line in the block.
 */
#include <string>
#include <vector>

class CharBuffer
{
public:
	CharBuffer(size_t bufferSize);
	std::vector<unsigned char> getCurrentLine() noexcept;
	// The addLine function returns true if the line can be added and
	// false if the buffer does not have the capacity to store the new line
	[[nodiscard]] bool addLine(std::string& line) noexcept;
	char getCurrentCharacter() noexcept {
		return internalBuffer[currentCharIdx];
		currentCharIdx++;
	}
	void addCharacter(unsigned char c) noexcept {
		internalBuffer[actualSize] = c;
		actualSize++;
	};
	bool endOfBuffer() noexcept { return currentCharIdx >= actualSize; }

private:
	std::vector<unsigned char> internalBuffer;
	size_t currentCharIdx;
	size_t capacity;
	size_t actualSize;
};
#endif // CHARBUFFER_H
