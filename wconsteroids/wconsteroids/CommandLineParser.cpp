#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "CommandLineParser.h"
#include "CmdLineFileExtractor.h"
#include "Executionctrlvalues.h"
#include "UtilityTimer.h"

// All operating sustem conditional code is at the top so that it can be easily
// found. The public interfaces immediately follow.

#ifdef _WIN32
static const std::size_t MinimumCommandLineCount = 1;
#else
// On Linux and Unix argv[0] is the program name so a minimum of 2 arguments
static const std::size_t MinimumCommandLineCount = 2;
#endif

std::string CommandLineParser::messageProgramName()
{
	std::string programName =
#ifdef _WIN32
		"wconsteriods"
#else
		// On Linux and Unix argv[0] is the program name;
		(argCount != 0) ? args[0] : "wconsteriods"
#endif
		;

	return programName;
}

/*
 * Begin public interface.
 */
CommandLineParser::CommandLineParser(int argc, char* argv[],
	std::string progVersion)
	: args{ {} },
	version{ std::move(progVersion) },
	options{ ProgramOptions() },
	doubleDashArgs{
		{ "--bytes", options.byteCount },
		{ "--chars", options.charCount },
		{ "--lines", options.lineCount },
		{ "--max-line-length", options.maxLineWidth },
		{ "--words", options.wordCount }
	},
	singleDashArgs{
		{ 'c', options.byteCount },
		{ 'm', options.charCount },
		{ 'l', options.lineCount },
		{ 'L', options.maxLineWidth },
		{ 'w', options.wordCount }
	},
	NotFlagsArgs{ {} },
	useDefaultFlags{ true }
{
	// Start at one to remove the program name
	for (int i = 1; i < argc; i++)
	{
		std::string_view arg(argv[i]);
		args.push_back(arg);
	}
}

bool CommandLineParser::parse(ExecutionCtrlValues& execVars)
{
	UtilityTimer stopWatch;
	// There is no way to determine if -t has been used at this point
	// so start the timer anyway.
	stopWatch.startTimer();

	bool hasFiles = false;

	if (args.size() + 1 < MinimumCommandLineCount)
	{
		ShowHelpMessage doHelp("Call printHelpMessage");
		throw doHelp;
	}

	extractAllArguments();
	if (useDefaultFlags)
	{
		SetDefaultOptionsWhenNoFlags();
	}

	findAllFilesToProcess(execVars);
	execVars.options = options;

	if (options.enableExecutionTime)
	{
		stopWatch.stopTimerAndReport("command line parsing at ");
	}

	return hasFiles = execVars.filesToProcess.size() != 0;
}

void CommandLineParser::printHelpMessage()
{
	std::cerr << "\n" << messageProgramName() <<
		" file name or file type specification (*.ext)\n"
		"Otions:\n"
		"\t-c, --bytes print the byte counts\n"
		"\t-m, --chars print the character counts\n"
		"\t-l, --lines print the newline counts\n"
		"\t-t, --time-execution print the execution time of the program\n"
		"\t-L, --max-line-length print the length of the longest line\n"
		"\t-w, --words print the word counts\n"
		"\t--help display this help and exit\n"
		"\t--version output version information and exit\n"
		"\t-R, --subdirectories all files in the"
		" directory as well as sub directories\n"
		"\tBy default the -c -l and -w flags are set, setting any"
		" flag requires all flags you want to be set.\n";
	// flush the buffer to make sure the entire message is visible
	std::cerr << std::flush;
}

void CommandLineParser::printVersion()
{
	std::cout << messageProgramName() << ": version: " << version << "\n"
		"Packaged by Chernick Consulting\n"
		"License GPLv3+: GNU GPL version 3 or later"
		" <http://gnu.org/licenses/gpl.html>.\n"
		"This is free software : you are free to change and redistribute it.\n"
		"\tThere is NO WARRANTY, to the extent permitted by law.\n"
		"\nWritten by Paul A. Chernick\n";
}

void CommandLineParser::findAllFilesToProcess(ExecutionCtrlValues& execVars)
{
	bool searchSubDirs = options.recurseSubDirectories;
	CmdLineFileExtractor fileExtractor(NotFlagsArgs, searchSubDirs);
	fileExtractor.findAllRequiredFiles();
	execVars.filesToProcess = fileExtractor.getFileList();
	execVars.fileSpecTypes = fileExtractor.getFileTypeList();
}

void CommandLineParser::extractAllArguments()
{
	// start after the program name.
	for (std::size_t i = 0; i < args.size(); i++)
	{
		if (args[i][0] == '-')
		{
			if (args[i][1] == '-')
			{
				processDoubleDashOptions(args[i]);
			}
			else
			{
				processSingleDashOptions(args[i]);
			}
		}
		else
		{
			NotFlagsArgs.push_back(std::move(args[i]));
		}
	}
}

/*
 * Flags starting with -- are full strings that need to be processed
 * as strings.
 */
void CommandLineParser::processDoubleDashOptions(std::string_view currentArg)
{
	auto flag = doubleDashArgs.find(currentArg);
	if (flag != doubleDashArgs.end())
	{
		(*flag).second = true;
		useDefaultFlags = false;
		return;
	}

	// The following switches require alternate handling
	if (currentArg == "--subdirectories")
	{
		// Since this is not a column switch it does not affect the default 
		options.recurseSubDirectories = true;
		return;
	}

	if (currentArg == "--time-execution")
	{
		// Since this is not a column switch it does not affect the default 
		options.enableExecutionTime = true;
		return;
	}

	if (currentArg == "--help")
	{
		ShowHelpMessage doHelp("Call printHelpMessage");
		throw doHelp;
	}

	if (currentArg == "--version")
	{
		showVersions sv("Call printVersion");
		throw sv;
	}

	std::cerr << "Unknown flag: " << currentArg << "\n";
}

/*
 * Each character needs to be processed independently.
 */
void CommandLineParser::processSingleDashOptions(std::string_view currentArg)
{
	for (std::size_t i = 1; i < currentArg.size(); i++)
	{
		auto thisOption = singleDashArgs.find(currentArg[i]);
		if (thisOption != singleDashArgs.end())
		{
			(*thisOption).second = true;
			useDefaultFlags = false;
		}
		else
		{
			switch (currentArg[i])
			{
			case 'R':
				// Since this is not a column switch it does not affect the
				// default  
				options.recurseSubDirectories = true;
				continue;
			case 't':
				// Since this is not a column switch it does not affect the
				// default  
				options.enableExecutionTime = true;
				continue;
			default:
				std::cerr << "Unknown flag: " << currentArg[i] << "\n";
				continue;
			}
		}
	}
}

void CommandLineParser::SetDefaultOptionsWhenNoFlags()
{
	// Based on the default functionality of the wc program.
	options.byteCount = true;
	options.wordCount = true;
	options.lineCount = true;
}

