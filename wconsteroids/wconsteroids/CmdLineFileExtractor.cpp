#include <doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <iterator>
#include <ranges>
#include <regex>
#include <string>
#include <string_view>
#include <vector>
#include "CmdLineFileExtractor.h"
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

// std::filesystem can make lines very long.
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
 * The booleans in the SubDirNode struct indicate the phase of processing completed.
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
// fileExtentions needs to be a vector string, there are bugs introduced
// when it is a string_view, at least in MSVC++ 2022.
static std::vector<std::string> fileNamePatterns;
static std::vector<std::string_view> nonFlagArgs;
static std::vector<SubDirNode> subDirectories;

/*
 * Search the current directory for sub directories.
 */
static auto findSubDirs(const SubDirNode& currentDir)
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
    std::ranges::copy(subdirs, std::back_inserter(newSubDirs));
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

static const std::string questionMarkReplacement("[a-zA-Z0-9:$@#-_./]");
static const std::string starReplacement(questionMarkReplacement + "*");

static std::string convertWildCards(std::string possiblePattern)
{
	std::string regexString;
	try
	{
		std::regex qmark("\\?");
		regexString = std::regex_replace(possiblePattern, qmark, questionMarkReplacement);
		std::regex asterisk("\\*");
		regexString = std::regex_replace(regexString, asterisk, starReplacement);
	}
	catch (std::regex_error& re)
	{
		std::cerr << "Programmer Error: Regex Error: " << re.what() << "\n";
		std::cerr << possiblePattern << "\n";
		throw;
	}

	return regexString;
}

static bool containsWildCard(std::string_view fileSpec)
{
	// 12/22/2022 Expanded definition of wild cards to include '?'.
	return fileSpec.find('*') != std::string::npos
		|| fileSpec.find('?') != std::string::npos;
}

// 12/22/2022 Use pattern matching of files rather than matching file extensions.
static auto searchDirectoryForFilesByPattern(SubDirNode currentDir,
	std::string partialFileSpec)
{
	auto is_missing = [](const std::string& branch) {
		return std::ranges::find(fileList, branch) == fileList.end();
	};

	std::vector<std::string> newFiles = {};
	std::string patternString = convertWildCards(partialFileSpec);
	// Handle any programmer regex errors.
	try
	{
		std::string dir = currentDir.fileSpec.string();
		std::regex pattern(patternString);
		auto is_match = [pattern](auto f) {
			return std::regex_search(f.path().string(), pattern);
		};

		auto files = fsys::directory_iterator{ currentDir.fileSpec }
			| std::views::filter([](auto& f) { return f.is_regular_file() ||
				f.is_character_file(); })
			| std::views::filter(is_match)
			| std::views::transform([](auto& f) { return f.path().string(); })
			| std::views::filter(is_missing);

		std::ranges::copy(files, std::back_inserter(newFiles));
	}
	catch (std::regex_error& re)
	{
		std::cerr << "Programmer Error: Regex Error: " << re.what() << "\n";
		std::cerr << patternString << "\n";
		throw;
	}

	return newFiles;
}

static void searchDirectoryForFiles(SubDirNode currentDir)
{
	// In the case of wildcard specification add the files to the file list in
	// the order they were specified.
	for (auto fileNamePattern : fileNamePatterns)
	{
		std::vector<std::string> newFiles =
			searchDirectoryForFilesByPattern(currentDir, fileNamePattern);
		std::copy(newFiles.begin(), newFiles.end(), std::back_inserter(fileList));
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

static std::vector<std::string> getFileNamePatterns()
{
	std::vector<std::string> fileTypes;
	std::vector<std::string_view> fileSpecTypes = findAllFileTypeSpecs();

	for (auto fileTypeSpec : fileSpecTypes)
	{
		fsys::path tempPath(fileTypeSpec);
		std::string fileExtension = tempPath.extension().string();
		if (!fileExtension.empty())
		{
			fileTypes.push_back(fileExtension);
		}
	}

	return fileTypes;
}

static void addListedFilesToFileList()
{
	for (auto fileSpec : nonFlagArgs)
	{
		std::string fSpec(fileSpec);
		if (!containsWildCard(fileSpec))
		{
			fileList.push_back(fSpec);
		}
	}
}

static void findAllInputFiles()
{
	// if there is nothing to search for quit.
	if (!SearchSubDirs || fileNamePatterns.empty())
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
	fileNamePatterns.clear();
	fileNamePatterns = getFileNamePatterns();
	addListedFilesToFileList();
	findAllInputFiles();
}

std::vector<std::string> CmdLineFileExtractor::getFileList() const noexcept
{
	return fileList;
}
