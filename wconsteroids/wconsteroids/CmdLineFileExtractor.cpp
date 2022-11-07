#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include "CmdLineFileExtractor.h"

/*
 * The code here expands wild card file specifications such as "*.cpp" and
 * "*.h" into lists of file names to process. If the -R or --subdirectories
 * command line flag is true then it searches through the current file
 * hierarchy for matching files as well.
 */
 
 /*
 * Internal variables and functions so that #include <fileSystem> does
 * not occur every where. It is only needed in this implementation file.
 * Normally I put the public interface functions first, but the static
 * code needs to be defined before that.
 */

static bool SearchSubDirs;
static std::vector<std::string> fileList;
static std::vector<std::string> fileExtentions;
static std::vector<std::string> nonFlagArgs;
static std::vector<std::string> subDirectories;

namespace fsys = std::filesystem;


static bool containsWildCard(std::string fileSpec)
{
	return fileSpec.find('*') != std::string::npos;
}

static std::string getFileExtention(std::string fname)
{
	size_t lastDotLocation = fname.find_last_of('.');
	if (lastDotLocation != std::string::npos)
	{
		return fname.substr(lastDotLocation);
	}

	std::string emptyReturn = "";
	return emptyReturn;
}

static bool isASpecifiedFileType(std::string notAFlag) noexcept
{
	// Exclude file type specifications since they are not actual file names
	if (containsWildCard(notAFlag))
	{
		return false;
	}

	std::string fileExtension = getFileExtention(notAFlag);
	if (fileExtension.empty())
	{
		return false;
	}

	auto expectedType = std::find(fileExtentions.begin(),
		fileExtentions.end(), fileExtension);

	return expectedType != fileExtentions.end();
}

static void demo_status(const fsys::path& p, fsys::file_status s)
{
	std::cout << p;
	// alternative: switch(s.type()) { case fs::file_type::regular: ...}
	if (fsys::is_regular_file(s)) std::cout << " is a regular file\n";
	if (fsys::is_directory(s)) std::cout << " is a directory\n";
	if (fsys::is_block_file(s)) std::cout << " is a block device\n";
	if (fsys::is_character_file(s)) std::cout << " is a character device\n";
	if (fsys::is_fifo(s)) std::cout << " is a named IPC pipe\n";
	if (fsys::is_socket(s)) std::cout << " is a named IPC socket\n";
	if (fsys::is_symlink(s)) std::cout << " is a symlink\n";
	if (!fsys::exists(s)) std::cout << " does not exist\n";
}

static void searchDirectorForFiles(fsys::path cwd)
{
	std::vector<fsys::path> subDirectories;
	for (auto it = fsys::directory_iterator(cwd);
		it != fsys::directory_iterator(); ++it)
	{
		if (it->is_directory())
		{
			subDirectories.push_back(it->path());
		}
		else
		{
			if (it->is_regular_file() || it->is_character_file())
			{
				std::string fileName{ it->path().generic_string() };
				if (isASpecifiedFileType(fileName))
				{
					fileList.push_back(fileName);
				}
			}
		}
	}

	if (SearchSubDirs)
	{
		for (auto SubDir : subDirectories)
		{
			searchDirectorForFiles(SubDir);
		}
	}
}

static std::vector<std::string> findAllFileTypeSpecs()
{
	std::vector<std::string> fileSpecTypes;
	// Get all the file specifications and store them in the
	// vector of file specifications. Remove the file specification
	// so that it isn't added to the list of files.
	for (auto notAFlag : nonFlagArgs)
	{
		if (containsWildCard(notAFlag))
		{
			fileSpecTypes.push_back(notAFlag);
		}
	}

	return fileSpecTypes;
}

static std::vector<std::string> getFileTypes()
{
	std::vector<std::string> fileTypes;
	std::vector<std::string> fileSpecTypes = findAllFileTypeSpecs();

	for (auto fileTypeSpec : fileSpecTypes)
	{
		std::string fileExtention = getFileExtention(fileTypeSpec);
		if (!fileExtention.empty())
		{
			fileTypes.push_back(fileExtention);
		}
	}

	return fileTypes;
}

static void addListedFilesToFileList()
{
	for (auto fileSpec : nonFlagArgs)
	{
		if (isASpecifiedFileType(fileSpec))
		{
			fileList.push_back(fileSpec);
		}
	}
}

static void findAllInputFiles()
{
	// if there is nothing to search for quit.
	if (!SearchSubDirs && fileExtentions.size() == 0)
	{
		return;
	}

	// Start in the current working directory  (cwd for non Linux users).
	std::filesystem::path cwd = fsys::current_path();
	searchDirectorForFiles(cwd);
}

/*
 * Begin public interfaces
 */

CmdLineFileExtractor::CmdLineFileExtractor(
	std::vector<std::string> NonFlagArgs,
	bool searchSubDirs)
{
	// copy is ok, we are not worried about performance here.
	// This class should be instantiated only once per program
	// run. It is only needed by the command line parser.
	nonFlagArgs = NonFlagArgs;
	SearchSubDirs = searchSubDirs;
}

void CmdLineFileExtractor::findAllRequiredFiles() noexcept
{
	fileExtentions = getFileTypes();
	addListedFilesToFileList();
	findAllInputFiles();
}

std::vector<std::string> CmdLineFileExtractor::getFileList() const noexcept
{
	return fileList;
}

std::vector<std::string> CmdLineFileExtractor::getFileTypeList() const noexcept
{
	return fileExtentions;
}

