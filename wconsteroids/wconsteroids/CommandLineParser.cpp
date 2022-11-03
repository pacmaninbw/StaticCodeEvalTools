#include <iostream>
#include <string>
#include <vector>
#include <program_options.hpp>
#include "CommandLineParser.h"
#include "Executionctrlvalues.h"

namespace ProgOpts = boost::program_options;

static ProgOpts::options_description desc("Allowed options");
static ProgOpts::variables_map vm;

static void initProgramOptions()
{
	bool byteCount;
	bool charCount;
	bool lineCount;
	desc.add_options()
		("help", "produce help message")
		("compression", ProgOpts::value<int>(), "set compression level")
		("c, bytes", ProgOpts::value<bool>(&byteCount)->default_value(false),
			"print the byte counts")
		("m, chars", ProgOpts::value<bool>(&charCount)->default_value(true),
			"print the character counts"),
		("l, lines, nolines", ProgOpts::value<bool>(&lineCount)->default_value(true),
			"print the newline counts")
		;
}

CommandLineParser::CommandLineParser(int argc, char* argv[])
	: argCount { argc }, args{ argv }
{
	initProgramOptions();
}

bool CommandLineParser::parse(ExecutionCtrlValues& execVars)
{
	constexpr size_t MinimumCommandLineCount = 2;	// Program name plus file name

	bool hasFiles = false;

	if (argCount < MinimumCommandLineCount)
	{
		std::cout << desc << "\n";
	}

	return hasFiles;
}
