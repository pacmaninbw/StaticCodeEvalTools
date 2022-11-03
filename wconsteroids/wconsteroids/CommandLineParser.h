#ifndef COMMANDlINEPARSER_H
#define COMMANDlINEPARSER_H

#include "Executionctrlvalues.h"

class CommandLineParser
{
public:
	CommandLineParser(int argc, char* argv[]);
	bool parse(ExecutionCtrlValues& execVars);

private:
	std::vector<std::string> arguments;
	char** args;
	int argCount;
};

#endif // COMMANDlINEPARSER_H