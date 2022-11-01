#ifndef	FILEREADER_H
#define FILEREADER_H

#include <string>
#include "CharBuffer.h"

class FileReader
{
public:
	FileReader(std::string inFileName);
	~FileReader();
	CharBuffer* readBlockOfText();
	bool atEndOfFile();

private:
	std::string fileName;
	bool endOfFileEncountered;
};

#endif // FILEREADER_H
