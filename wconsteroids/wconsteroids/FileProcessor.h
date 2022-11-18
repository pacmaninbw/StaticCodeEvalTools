#ifndef FILE_PROCESSOR_H
#define FILE_PROCESSOR_H

#include <string>
#include <vector>
#include <fstream>

class FileStatistics;
class ProgramOptions;

class FileProcessor
{
public:
	FileProcessor(std::vector<std::string> filesToProcess, const ProgramOptions& progOptions);
	~FileProcessor() = default;
	std::string processAllFiles() noexcept;

protected:
	void processLoop(std::ifstream& inStream, FileStatistics& statistics) noexcept;
	std::string processFile(std::string fileName, FileStatistics& totalStats);

private:
	std::vector<std::string> fileNames;
	// The program options are necessary to know what to output.
	const ProgramOptions& options;
};

#endif // FILE_PROCESSOR_H
