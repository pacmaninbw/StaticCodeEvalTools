#ifndef	FILEREADER_H
#define FILEREADER_H

/*
 * Inputs a single file collecting the contents of the file in a buffer that
 * is returned to the caller. Also collects basic information about the file
 * such as the number of lines in the file and the number of characters in
 * the file.
 */

#include <string>
#include "CharBuffer.h"

class FileReader
{
public:
	FileReader(std::string inFileName);
	~FileReader();
	CharBuffer* readBlockOfText();
	bool atEndOfFile();
	size_t getLineCount() { return lineCount; }
	size_t getCharCount() { return charCount; }

private:
	std::string fileName;
	bool endOfFileEncountered;
	size_t charCount;
	size_t lineCount;
};

#endif // FILEREADER_H
