#ifndef COMMAND_LINE_FILE_EXTRACTOR_H
#define COMMAND_LINE_FILE_EXTRACTOR_H

/*
 * This class has one purpose, to build a list of files to process for
 * the calling program. It expands wild card strings. It searches the
 * current working directory for files of the specified types. If the
 * search subdirs bool is set it will also search the sub directories
 * of the current working directory. The list of files to process will
 * be full file specifications so that all the proper files are read
 * only once.
 */

#include <string>
#include <vector>

class CmdLineFileExtractor
{
public:
	CmdLineFileExtractor(std::vector<std::string> NonFlagArgs, bool searchSubDirs);
	void findAllRequiredFiles();
	std::vector<std::string> getFileList() const noexcept
	{
		return fileList;
	}
	std::vector<std::string> getFileTypeList() const noexcept
	{
		return fileTypeList;
	}

protected:
	void findAllInputFiles();
	std::vector<std::string> findAllFileTypeSpecs();
	std::vector<std::string> getFileTypes();


private:
	bool SearchSubDirs;
	std::vector<std::string> fileList;
	std::vector<std::string> fileTypeList;
	std::vector<std::string> nonFlagArgs;
	std::vector<std::string> subDirectories;
};

#endif // COMMAND_LINE_FILE_EXTRACTOR_H