#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "CommandLineParser.h"
#include "CmdLineFileExtractor.h"
#include "Executionctrlvalues.h"

#ifdef _WIN32
// File Name or File type specification
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
	// Initialize all options to false;
	memset(&options, 0, sizeof(options));
}

void CommandLineParser::findAllFilesToProcess(ExecutionCtrlValues& execVars)
{
	bool searchSubDirs = execVars.options.recurseSubDirectories;
	CmdLineFileExtractor fileExtractor(NotFlags, searchSubDirs);
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
			NotFlags.push_back(args[i]);
		}
	}

	return flagCount;
}

bool CommandLineParser::parse(ExecutionCtrlValues& execVars)
{
	bool hasFiles = true;

	if (argCount < MinimumCommandLineCount)
	{
		HelpMe doHelp("Call printHelpMessage");
		throw doHelp;
	}

	unsigned int flagCount = extractAllArguments();
	if (!flagCount)
	{
		SetDefaultOptionsWhenNoFlags();
	}

	findAllFilesToProcess(execVars);

	execVars.options = options;
	return hasFiles;
}

void CommandLineParser::printHelpMessage()
{
	std::string programName =
#ifdef _WIN32
		"wconsteriods"
#else
		// On Linux and Unix argv[0] is the program name;
		(argCount != 0) ? args[0] : "wconsteriods"
#endif
		;
	std::cerr << "\n" << programName;
	for (auto line : helpMessage)
	{
		std::cerr << line;
	}
	// flush the buffer to make sure the entire message is visible
	std::cerr << std::endl;
}

void CommandLineParser::printVersion()
{
	std::cout << args[0] << ": version: " << version << "\n";
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
		HelpMe doHelp("Call printHelpMessage");
		throw doHelp;
	}

	if (strncmp(currentArg, "--version", strlen("--version")) == 0)
	{
		showVersions sv("Call printVersion");
		throw sv;
	}

	if (strncmp(currentArg, "--files0-from",
		strlen("--files0-from")) == 0)
	{
		std::cerr << "--files0-from Not implemented yet\n";
		return;
	}

	std::cerr << "Unknown flag: " << currentArg << "\n";
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

	singleDashArgs.insert({ 'c', options.byteCount });
	singleDashArgs.insert({ 'm', options.charCount });
	singleDashArgs.insert({ 'l', options.lineCount });
	singleDashArgs.insert({ 'L', options.maxLineWidth });
	singleDashArgs.insert({ 'w', options.wordCount });
	singleDashArgs.insert({ 'p', options.percentages });
	singleDashArgs.insert({ 'R', options.recurseSubDirectories });
}

void CommandLineParser::initHelpMessage()
{
	helpMessage.push_back(" file name or file type specification (*.ext)\n");
	helpMessage.push_back("Otions:\n");
	helpMessage.push_back("\t-c, --bytes print the byte counts\n");
	helpMessage.push_back("\t-m, --chars print the character counts\n");
	helpMessage.push_back("\t-l, --lines print the newline counts\n");
	std::string veryLongLine = "\t--files0-from=F read input from the files ";
	veryLongLine +=
		"specified by NUL - terminated names in file F; If F is - then read ";
	veryLongLine += "names from standard input\n";
	helpMessage.push_back(veryLongLine);
	helpMessage.push_back(
		"\t-L, --max-line-length print the length of the longest line\n");
	helpMessage.push_back("\t-w, --words print the word counts\n");
	helpMessage.push_back("\t--help display this help and exit\n");
	helpMessage.push_back("\t--version output version information and exit\n");
	helpMessage.push_back("\t--comment print comment count lines\n");
	helpMessage.push_back("\t--code print code line counts\n");
	helpMessage.push_back("\t--whitespace print whitespace count\n");
	veryLongLine = "\t-p --percentage print percentages of code"
		" per file and total\n";
	helpMessage.push_back(veryLongLine);
	veryLongLine = "\t-R, --subdirectories all files in the"
		" directory as well as sub directories\n";
	helpMessage.push_back(veryLongLine);
	veryLongLine = "\tBy default the -c -l and -w flags are set, setting any"
		" flag requires all flags you want to be set.\n";
	helpMessage.push_back(veryLongLine);
}

