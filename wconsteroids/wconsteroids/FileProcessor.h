#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <string>
#include "FileStatistics.h"
#include "FileReader.h"
#include "ProgramOptions.h"

class FileProcessor
{
public:
	FileProcessor(std::string inFileName, ProgramOptions& progOptions);
	~FileProcessor() = default;
	bool processFile();
	FileStatistics getStatistics();
	void mergeStatistics(FileStatistics& allFileStats);

private:
	std::string fileName;
	FileStatistics statistics;
	FileReader reader;
	// The program options are necessary to know what to outout.
	ProgramOptions& options;
};

#endif // FILEPROCESSOR_H
