#ifndef COMMANDlINEPARSER_H
#define COMMANDlINEPARSER_H

#include "Executionctrlvalues.h"

class CommandLineParser
{
public:
	CommandLineParser(int argc, char* argv[], std::string progVersion);
	bool parse(ExecutionCtrlValues& execVars);
	void printHelpMessage();
	void printVersion();

private:
	std::vector<std::string> arguments;
	char** args;
	int argCount;
	std::string version;
};

#endif // COMMANDlINEPARSER_H