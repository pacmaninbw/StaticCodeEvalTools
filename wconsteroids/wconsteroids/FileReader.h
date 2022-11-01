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

private:
	std::string fileName;
};

#endif // FILEREADER_H
