#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>
#include "CommandLineParser.h"
#include "CmdLineFileExtractor.h"
#include "Executionctrlvalues.h"
#include "UtilityTimer.h"
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

static std::string simplify_name(char *path)
{
	return std::filesystem::path{path ? path : "wconsteroids"}.filename().string();
}

/*
 * Begin public interface.
 */

/*
 * Turns all command line arguments into a vector of string_view
 * to make additional processing easier and more C++ like rather
 * than old style C code. Removes any dependency on cstring (string.h).
 */
CommandLineParser::CommandLineParser(int argc, char* argv[])
	: args(argv + 1, argv + argc)
{
	std::string programName = simplify_name(argv[0]);
	options.programName = programName;
}

bool CommandLineParser::parse(ExecutionCtrlValues& execVars)
{
	UtilityTimer stopWatch;

	execVars.initFromEnvironmentVariables();

	extractAllArguments();

	findAllFilesToProcess(execVars);
	execVars.options = options;

	if (options.enableExecutionTime)
	{
		stopWatch.stopTimerAndReport("command line parsing at ");
	}

	return execVars.filesToProcess.size() != 0;
}

void CommandLineParser::findAllFilesToProcess(ExecutionCtrlValues& execVars)
{
	bool searchSubDirs = options.recurseSubDirectories;
	CmdLineFileExtractor fileExtractor(NotFlagsArgs, searchSubDirs);
	fileExtractor.findAllRequiredFiles();
	execVars.filesToProcess = fileExtractor.getFileList();
}

void CommandLineParser::extractAllArguments()
{
	// Since argv[0] is removed in the constructor we
	// can perform a ranged for loop.
	for (auto arg: args)
	{
		if (arg[0] == '-')
		{
			options.processArgument(arg);
		}
		else
		{
			NotFlagsArgs.push_back(std::move(arg));
		}
	}

	options.SetDefaultOptionsWhenNoFlags();
}
