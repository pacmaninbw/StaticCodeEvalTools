#include "CommandLineParser.h"
#include <string>
#include <vector>
#include "cmdlineoutputtypes.h"

CommandLineParser::CommandLineParser(int argc, char* argv[])
	: argCount { argc }, args{ argv }
{

}

void CommandLineParser::parse()
{

}

CmdLineOutputTypes CommandLineParser::getOutputTypes()
{
	CmdLineOutputTypes outputTypes = CmdLineOutputTypes::Default;

	return outputTypes;
}