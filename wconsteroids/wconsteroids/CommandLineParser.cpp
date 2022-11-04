#include <iostream>
#include <string>
#include <vector>
#include "CommandLineParser.h"
#include "Executionctrlvalues.h"

static const size_t MinimumCommandLineCount = 2; // Program name plus file name
static std::vector<std::string> helpMessage;

static void initHelpMessage()
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

static void initProgramOptions()
{
	
}

CommandLineParser::CommandLineParser(int argc, char* argv[], std::string progVersion)
	: argCount { argc }, args{ argv }
{
	version = progVersion;
	initHelpMessage();
	initProgramOptions();

	for (size_t i = 0; i < argCount; i++)
		std::cout << args[i] << "\n";
}

bool CommandLineParser::parse(ExecutionCtrlValues& execVars)
{
	bool hasFiles = true;

	if (argCount < MinimumCommandLineCount)
	{
		printHelpMessage();
		hasFiles = false;
	}

	return hasFiles;
}

void CommandLineParser::printHelpMessage()
{
	std::cerr << "\n" << (argCount != 0) ? args[0] : "wconsteriods";
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