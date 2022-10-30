#ifndef COMMANDlINEPARSER_H
#define COMMANDlINEPARSER_H

#include <string>
#include <vector>
#include "cmdlineoutputtypes.h"

class CommandLineParser
{
public:
	CommandLineParser(int argc, char* argv[]);
	CmdLineOutputTypes getOutputTypes();
	void parse();

private:
	std::vector<std::string> arguments;
	char** args;
	int argCount;
};

#endif // COMMANDlINEPARSER_H