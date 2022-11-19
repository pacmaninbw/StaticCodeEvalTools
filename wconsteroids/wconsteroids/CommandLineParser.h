#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H

/*
 * Generic class to parse command lines. The public interface should not
 * require modifications for different programs. The help message will
 * need to change on a per program basis. The help message definition may
 * need to move to the ProgramOptions class.
 * 
 * This class should be portable to Windows, Linux and Unix operating
 * systems. There are ifdefs in the implementation for this purpose.
 */
#include <string_view>
#include <vector>
#include "Executionctrlvalues.h"
#include "ProgramOptions.h"

class CommandLineParser
{
public:
	CommandLineParser(int argc, char* argv[]);
	bool parse(ExecutionCtrlValues& execVars);

protected:
	void findAllFilesToProcess(ExecutionCtrlValues& execVars);
	void extractAllArguments();

private:
	std::vector<std::string_view> args;
	ProgramOptions options{};
	std::vector<std::string_view> NotFlagsArgs{};
};

#endif // COMMAND_LINE_PARSER_H
