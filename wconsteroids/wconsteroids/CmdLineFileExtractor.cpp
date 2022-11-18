#include <algorithm>
#include <iostream>
#include <filesystem>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include "CmdLineFileExtractor.h"

namespace fsys = std::filesystem;

/*
 * The code here expands wild card file specifications such as "*.cpp" and
 * "*.h" into lists of file names to process. If the -R or --subdirectories
 * command line flag is true then it searches through the current file
 * hierarchy for matching files as well.
 *
 * Originally the file search was implemented as a recursive algorithm, this
 * crashed on Windows 10 when searching for *.hpp in the boost library as a 
 * test case. The code has been rewritten as an iterative solution, first
 * find all the sub directories in the current directory and add them to the
 * sub directory list. Then for each sub directory in the sub directory list
 * search for more sub directories. When all the sub directories have been
 * searched for sub directories find the applicable files in each sub
 * directory.
 * 
 * The booleans in this class indicate the phase of processing completed.
 * The discovered value indicates whether the sub directory has been searched
 *		for more sub directories.
 * The searchedFiles flag indicates if the files in the directory have been
 *		added to the fileList vector.
 */
struct SubDirNode
{
	fsys::path fileSpec;
	bool discovered = false;
	bool searchedFiles = false;

	SubDirNode(fsys::path path)
		: fileSpec{ std::move(path) }
	{
	}

	bool operator==(const SubDirNode& other) const
	{
		return fileSpec == other.fileSpec;
	}
	bool operator!=(const SubDirNode& other) const = default;
};

/*
* Internal variables and functions so that #include <fileSystem> does
* not occur every where. It is only needed in this implementation file.
* Normally I put the public interface functions first, but the static
* code needs to be defined before that.
*/
static bool SearchSubDirs;
static std::vector<std::string> fileList;
static std::vector<std::string_view> fileExtentions;
static std::vector<std::string_view> nonFlagArgs;
static std::vector<SubDirNode> subDirectories;

/*
 * Search the current directory for sub directories.
 */
static auto findSubDirs(SubDirNode currentDir)
{
    auto is_missing = [](const SubDirNode& branch){
        return std::ranges::find(subDirectories, branch) == subDirectories.end();
    };

    auto subdirs = fsys::directory_iterator{currentDir.fileSpec}
        | std::views::filter([](auto& f){ return f.is_directory(); })
        | std::views::transform([](auto& f)->SubDirNode { return f.path(); })
        | std::views::filter(is_missing);

    // TODO (C++23?) return std::vector(subdirs);
    auto newSubDirs = std::vector<SubDirNode>{};
    for (auto d: subdirs) {
        newSubDirs.emplace_back(d);
    }
    return newSubDirs;
}

/*
 * A recursive algorithm can apparently cause a stack overflow on Windows 10 
 * so this is an iterative solution.
 */
static bool discoverSubDirs()
{
	bool discoveredPhaseCompleted = true;
	std::vector<SubDirNode> newSubDirs;

	for (std::size_t i = 0; i < subDirectories.size(); i++)
	{
		if (!subDirectories[i].discovered)
		{
			std::vector<SubDirNode> tempNewDirs = findSubDirs(subDirectories[i]);
			if (tempNewDirs.size())
			{
				discoveredPhaseCompleted = false;
				newSubDirs.insert(newSubDirs.end(), tempNewDirs.begin(),
					tempNewDirs.end());
			}
			subDirectories[i].discovered = true;
		}
	}

	// We are done searching the current level, append the new sub directories
	// to the old.
	subDirectories.insert(subDirectories.end(), newSubDirs.begin(), newSubDirs.end());
	return discoveredPhaseCompleted;
}

static void discoverAllSubDirs()
{
	bool discoveryPhaseCompleted = false;
	while (!discoveryPhaseCompleted)
	{
		discoveryPhaseCompleted = discoverSubDirs();
	}
}


static bool containsWildCard(std::string_view fileSpec)
{
	return fileSpec.find('*') != std::string::npos;
}

static std::string getFileExtention(std::string_view fname)
{
	std::string fileExtention = "";

	std::size_t lastDotLocation = fname.find_last_of('.');
	if (lastDotLocation != std::string::npos)
	{
		fileExtention = fname.substr(lastDotLocation);
	}

	return fileExtention;
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

	return std::find(fileExtentions.begin(),
		fileExtentions.end(), fileExtension) != fileExtentions.end();
}

static void searchDirectoryForFiles(SubDirNode currentDir)
{

	for (auto it = fsys::directory_iterator(currentDir.fileSpec);
		it != fsys::directory_iterator(); ++it)
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

static void searchAllDirectoriesForFiles()
{
	for (auto currentDir : subDirectories)
	{
		if (currentDir.discovered && !currentDir.searchedFiles)
		{
			searchDirectoryForFiles(currentDir);
			currentDir.searchedFiles = true;
		}
	}
}

static std::vector<std::string_view> findAllFileTypeSpecs()
{
	std::vector<std::string_view> fileSpecTypes;
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

static std::vector<std::string_view> getFileTypes()
{
	std::vector<std::string_view> fileTypes;
	std::vector<std::string_view> fileSpecTypes = findAllFileTypeSpecs();

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
		std::string fSpec(fileSpec);
		fileList.push_back(fSpec);
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
	if (subDirectories.empty())
	{
		SubDirNode root(cwd);
		root.discovered = (SearchSubDirs) ? false: true;
		subDirectories.push_back(root);
	}

	if (SearchSubDirs)
	{
		discoverAllSubDirs();
	}
	searchAllDirectoriesForFiles();
}

/*
 * Begin public interfaces
 */

CmdLineFileExtractor::CmdLineFileExtractor(
	std::vector<std::string_view> NonFlagArgs,
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

std::vector<std::string_view> CmdLineFileExtractor::getFileTypeList() const noexcept
{
	return fileExtentions;
}
