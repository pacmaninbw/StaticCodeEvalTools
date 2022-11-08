#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <string>
#include "FileStatistics.h"
#include "FileReader.h"

class FileProcessor
{
public:
	FileProcessor(std::string inFileName);
	~FileProcessor() = default;
	bool processFile();
	FileStatistics getStatistics();
	void mergeStatistics(FileStatistics& allFileStats);

private:
	std::string fileName;
	FileStatistics statistics;
	FileReader reader;
};

#endif // FILEPROCESSOR_H
