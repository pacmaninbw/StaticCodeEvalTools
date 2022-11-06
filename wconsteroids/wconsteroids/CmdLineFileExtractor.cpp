
#include <filesystem>
#include <string>
#include <vector>
#include "CmdLineFileExtractor.h"

CmdLineFileExtractor::CmdLineFileExtractor(
	std::vector<std::string> NonFlagArgs,
	bool searchSubDirs = false)
	: SearchSubDirs { searchSubDirs }
{
	// copy is ok, we are not worried about performance here.
	// This class should be instantiated only once per program
	// run. It is only needed by the command line parser.
	nonFlagArgs = NonFlagArgs;
}

std::vector<std::string> CmdLineFileExtractor::findAllFileTypeSpecs()
{
	std::vector<std::string> fileSpecTypes;
	// Get all the file specifications and store them in the
	// vector of file specifications.
	for (auto notAFlag : nonFlagArgs)
	{
		if (notAFlag.find('*') != std::string::npos)
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
		size_t lastDotLocation = fileTypeSpec.find_last_of('.');
		if (lastDotLocation != std::string::npos)
		{
			std::string fileType = fileTypeSpec.substr(lastDotLocation);
			fileTypes.push_back(fileType);
		}
	}

	return fileTypes;
}

void CmdLineFileExtractor::findAllRequiredFiles()
{
	fileTypeList = getFileTypes();
}
