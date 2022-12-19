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
 * 
 * What would be protected or private functions and variables are
 * implemented as static functions and variables within the
 * implementation .cpp file to limit the access to the file system to
 * only this class/module.
 */

#include <string>
#include <string_view>
#include <vector>

class CmdLineFileExtractor
{
public:
	CmdLineFileExtractor(std::vector<std::string_view> NonFlagArgs, bool searchSubDirs);
	void findAllRequiredFiles() noexcept;
	std::vector<std::string> getFileList() const noexcept;
};

#endif // COMMAND_LINE_FILE_EXTRACTOR_H