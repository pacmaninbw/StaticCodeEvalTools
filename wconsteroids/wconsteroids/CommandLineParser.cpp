#include <string>
#include <vector>
#include <program_options.hpp>
#include "CommandLineParser.h"
#include "cmdlineoutputtypes.h"

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

void CommandLineParser::parse()
{

}

CmdLineOutputTypes CommandLineParser::getOutputTypes()
{
	CmdLineOutputTypes outputTypes = CmdLineOutputTypes::Default;

	return outputTypes;
}

std::vector<std::string> CommandLineParser::getFileList()
{
	return fileList;
}