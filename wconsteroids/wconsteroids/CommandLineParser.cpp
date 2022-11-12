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

#ifdef _WIN32
static const size_t MinimumCommandLineCount = 1;
#else
// On Linux and Unix argv[0] is the program name so a minimum of 2 arguments
static const size_t MinimumCommandLineCount = 2;
#endif

CommandLineParser::CommandLineParser(int argc, char* argv[],
	std::string progVersion)
	: argCount { argc }, args{ argv }
{
	version = progVersion;
	initHelpMessage();
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

unsigned int CommandLineParser::extractAllArguments()
{
	unsigned int flagCount = 0;

	for (size_t i = 0; i < argCount; i++)
	{
		if (args[i][0] == '-')
		{
			if (args[i][1] == '-')
			{
				processDoubleDashOptions(args[i]);
				flagCount++;
			}
			else
			{
				processSingleDashOptions(args[i]);
				flagCount++;
			}
		}
		else
		{
			NotFlagsArgs.push_back(args[i]);
		}
	}

	return flagCount;
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

	unsigned int flagCount = extractAllArguments();
	if (!flagCount)
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
	std::cerr << "\n" << messageProgramName();
	for (auto line : helpMessage)
	{
		std::cerr << line;
	}
	// flush the buffer to make sure the entire message is visible
	std::cerr << std::endl;
}

void CommandLineParser::printVersion()
{
		std::cout << messageProgramName() << ": version: " << version << "\n";
		std::cout << "Packaged by Chernick Consulting\n";
		std::cout << "License GPLv3+: GNU GPL version 3 or later"
			" <http://gnu.org/licenses/gpl.html>.\n";
		std::cout << "This is free software : you are free to change and redistribute it.\n";
		std::cout << "\tThere is NO WARRANTY, to the extent permitted by law.\n";
		std::cout << "\nWritten by Paul A. Chernick\n";
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
		return;
	}

	// The following switches require alternate handling
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

#if 0
	// This wc flag will not be implemented.
	if (strncmp(currentArg, "--files0-from",
		strlen("--files0-from")) == 0)
	{
		std::cerr << "--files0-from Not implemented yet\n";
		return;
	}

	std::cerr << "Unknown flag: " << currentArg << "\n";
#endif
}

/*
 * Each character needs to be processed independently.
 */
void CommandLineParser::processSingleDashOptions(char* currentArg)
{
	for (size_t i = 1; i < std::strlen(currentArg); i++)
	{
		auto thisOption = singleDashArgs.find(currentArg[i]);
		if (thisOption != singleDashArgs.end())
		{
			(*thisOption).second = true;
		}
		else
		{
			std::cerr << "Unknown flag: " << currentArg[i] << "\n";
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
	doubleDashArgs.insert({ "--comment", options.commentCount });
	doubleDashArgs.insert({ "--code", options.codeCount });
	doubleDashArgs.insert({ "--whitespace", options.whitespaceCount });
	doubleDashArgs.insert({ "--percentage", options.percentages });
	doubleDashArgs.insert({ "--subdirectories", options.recurseSubDirectories });
	doubleDashArgs.insert({ "--time-execution", options.enableExecutionTime});

	singleDashArgs.insert({ 'c', options.byteCount });
	singleDashArgs.insert({ 'm', options.charCount });
	singleDashArgs.insert({ 'l', options.lineCount });
	singleDashArgs.insert({ 'L', options.maxLineWidth });
	singleDashArgs.insert({ 'w', options.wordCount });
	singleDashArgs.insert({ 'p', options.percentages });
	singleDashArgs.insert({ 'R', options.recurseSubDirectories });
	singleDashArgs.insert({ 't', options.enableExecutionTime });
}

void CommandLineParser::initHelpMessage()
{
	std::string veryLongLine;
	helpMessage.push_back(" file name or file type specification (*.ext)\n");
	helpMessage.push_back("Otions:\n");
	helpMessage.push_back("\t-c, --bytes print the byte counts\n");
	helpMessage.push_back("\t-m, --chars print the character counts\n");
	helpMessage.push_back("\t-l, --lines print the newline counts\n");
	helpMessage.push_back(
		"\t-t, --time-execution print the execution time of the program");
	helpMessage.push_back(
		"\t-L, --max-line-length print the length of the longest line\n");
	helpMessage.push_back("\t-w, --words print the word counts\n");
	helpMessage.push_back("\t--help display this help and exit\n");
	helpMessage.push_back("\t--version output version information and exit\n");
#if 0
	// Not currently implemented
	helpMessage.push_back("\t--comment print comment count lines\n");
	helpMessage.push_back("\t--code print code line counts\n");
	helpMessage.push_back("\t--whitespace print whitespace count\n");
	veryLongLine = "\t-p --percentage print percentages of code"
		" per file and total\n";
	helpMessage.push_back(veryLongLine);
#endif
	veryLongLine = "\t-R, --subdirectories all files in the"
		" directory as well as sub directories\n";
	helpMessage.push_back(veryLongLine);
	veryLongLine = "\tBy default the -c -l and -w flags are set, setting any"
		" flag requires all flags you want to be set.\n";
	helpMessage.push_back(veryLongLine);
}

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
