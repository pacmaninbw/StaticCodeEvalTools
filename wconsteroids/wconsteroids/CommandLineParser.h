#ifndef COMMANDlINEPARSER_H
#define COMMANDlINEPARSER_H

#include "Executionctrlvalues.h"
#include "HelpMe.h"

class CommandLineParser
{
public:
	CommandLineParser(int argc, char* argv[], std::string progVersion);
	bool parse(ExecutionCtrlValues& execVars);
	void printHelpMessage();
	void printVersion();

protected:
	void processSingleDashOptions(char *currentArg);
	void processDoubleDashOptions(char* currentArg);
	void SetDefaultOptionsWhenNoFlags();


private:
	std::vector<std::string> arguments;
	char** args;
	int argCount;
	std::string version;
	ProgramOptions options;
};

#endif // COMMANDlINEPARSER_H