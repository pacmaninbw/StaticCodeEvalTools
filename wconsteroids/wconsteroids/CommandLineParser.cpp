#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
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

CommandLineParser::CommandLineParser(int argc, char* argv[],
	std::string progVersion)
	: argCount{ argc },
	args{ argv },
	useDefaultFlags{ true }
{
	version = progVersion;
	initDashMaps();
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
	for (std::size_t i = 0; i < argCount; i++)
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
			NotFlagsArgs.push_back(args[i]);
		}
	}
}

bool CommandLineParser::parse(ExecutionCtrlValues& execVars)
{
	UtilityTimer stopWatch;
	// There is no way to determine if -t has been used at this point
	// so start the timer anyway.
	stopWatch.startTimer();

	bool hasFiles = false;

	if (argCount < MinimumCommandLineCount)
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

/*
 * Flags starting with -- are full strings that need to be processed
 * as strings.
 */
void CommandLineParser::processDoubleDashOptions(char* currentArg)
{
	auto flag = doubleDashArgs.find(currentArg);
	if (flag != doubleDashArgs.end())
	{
		(*flag).second = true;
		useDefaultFlags = false;
		return;
	}

	// The following switches require alternate handling
	if (strncmp(currentArg, "--subdirectories", strlen("--subdirectories")) == 0)
	{
		// Since this is not a column switch it does not affect the default 
		options.recurseSubDirectories = true;
		return;
	}

	if (strncmp(currentArg, "--time-execution", strlen("--time-execution")) == 0)
	{
		// Since this is not a column switch it does not affect the default 
		options.enableExecutionTime = true;
		return;
	}

	if (strncmp(currentArg, "--help", strlen("--help")) == 0)
	{
		ShowHelpMessage doHelp("Call printHelpMessage");
		throw doHelp;
	}

	if (strncmp(currentArg, "--version", strlen("--version")) == 0)
	{
		showVersions sv("Call printVersion");
		throw sv;
	}

	std::cerr << "Unknown flag: " << currentArg << "\n";
}

/*
 * Each character needs to be processed independently.
 */
void CommandLineParser::processSingleDashOptions(char* currentArg)
{
	for (std::size_t i = 1; i < std::strlen(currentArg); i++)
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

void CommandLineParser::initDashMaps()
{
	doubleDashArgs.insert({ "--bytes", options.byteCount });
	doubleDashArgs.insert({ "--chars", options.charCount });
	doubleDashArgs.insert({ "--lines", options.lineCount });
	doubleDashArgs.insert({ "--max-line-length", options.maxLineWidth });
	doubleDashArgs.insert({ "--words", options.wordCount });

	singleDashArgs.insert({ 'c', options.byteCount });
	singleDashArgs.insert({ 'm', options.charCount });
	singleDashArgs.insert({ 'l', options.lineCount });
	singleDashArgs.insert({ 'L', options.maxLineWidth });
	singleDashArgs.insert({ 'w', options.wordCount });
}
