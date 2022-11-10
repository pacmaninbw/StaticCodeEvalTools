#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <string>
#include <fstream>

#include "FileStatistics.h"
#include "ProgramOptions.h"

class FileProcessor
{
public:
	FileProcessor(std::string inFileName, ProgramOptions& progOptions);
	~FileProcessor() = default;
	bool processFile();
	FileStatistics getStatistics();
	void mergeStatistics(FileStatistics& allFileStats);

protected:
	void processLoop(std::ifstream& inStream) noexcept;

private:
	std::string fileName;
	FileStatistics statistics;
	// The program options are necessary to know what to outout.
	ProgramOptions& options;
};

#endif // FILEPROCESSOR_H
