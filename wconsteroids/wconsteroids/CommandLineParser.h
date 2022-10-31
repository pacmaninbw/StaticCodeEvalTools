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
	std::vector<std::string> getFileList();
	bool parse();

private:
	std::vector<std::string> arguments;
	char** args;
	int argCount;
	std::vector<std::string> fileList;
};

#endif // COMMANDlINEPARSER_H