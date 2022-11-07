
#include <filesystem>
#include <string>
#include <vector>
#include "CmdLineFileExtractor.h"

/*
 * Internal variables and functions so that #include <fileSystem> does
 * not occur every where. It is only needed in this implementation file.
 */

static std::vector<std::string> staticFileExtentions;
static bool staticSearchSubDirs;

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
	// Exclude file type specifications
	if (containsWildCard(notAFlag))
	{
		return false;
	}

	std::string fileExtension = getFileExtention(notAFlag);
	if (fileExtension.empty())
	{
		return false;
	}

	auto expectedType = std::find(staticFileExtentions.begin(),
		staticFileExtentions.end(), fileExtension);

	return expectedType != staticFileExtentions.end();
}

static std::vector<std::string> mergerFileLists(
	std::vector<std::string> oldList,
	std::vector<std::string> additions
) noexcept
{
	std::vector < std::string> newFileList(oldList.size() + additions.size());
	newFileList.insert(newFileList.end(), oldList.begin(), oldList.end());
	newFileList.insert(newFileList.end(), additions.begin(), additions.end());

	return newFileList;
}

static std::vector<std::string> searchDirectorForFiles(std::filesystem::path cwd)
{
	std::vector <std::string> tempFilesToProcess;
	std::vector<std::filesystem::path> subDirectories;

	return tempFilesToProcess;
}

/*
 * Begin public interfaces
 */

CmdLineFileExtractor::CmdLineFileExtractor(
	std::vector<std::string> NonFlagArgs,
	bool searchSubDirs = false)
	: SearchSubDirs { searchSubDirs }
{
	// copy is ok, we are not worried about performance here.
	// This class should be instantiated only once per program
	// run. It is only needed by the command line parser.
	nonFlagArgs = NonFlagArgs;
	staticSearchSubDirs = searchSubDirs;
}

void CmdLineFileExtractor::findAllRequiredFiles()
{
	fileTypeList = getFileTypes();
	std::copy(fileTypeList.begin(), fileTypeList.end(),
		std::back_inserter(staticFileExtentions));
	addListedFilesToFileList();
	findAllInputFiles();
}

std::vector<std::string> CmdLineFileExtractor::findAllFileTypeSpecs()
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

std::vector<std::string> CmdLineFileExtractor::getFileTypes()
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

void CmdLineFileExtractor::addListedFilesToFileList()
{
	for (auto fileSpec : nonFlagArgs)
	{
		if (isASpecifiedFileType(fileSpec))
		{
			fileList.push_back(fileSpec);
		}
	}
}

void CmdLineFileExtractor::findAllInputFiles()
{
	// if there is nothing to search for quit.
	if (!SearchSubDirs && fileTypeList.size() == 0)
	{
		return;
	}

	// Start in the current working directory  (cwd foor non Linux users).
	std::filesystem::path cwd = std::filesystem::current_path();
	fileList = mergerFileLists(fileList, searchDirectorForFiles(cwd));
}
