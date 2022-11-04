#ifndef COMMANDlINEPARSER_H
#define COMMANDlINEPARSER_H

#include <unordered_map>
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
	void initDashMaps();
	void nonFlagCmdLineInput(char* currentArg);

private:
	std::vector<std::string> arguments;
	char** args;
	int argCount;
	std::string version;
	ProgramOptions options;
	std::unordered_map<std::string, bool&> doubleDashArgs;
	std::unordered_map<char, bool&> singleDashArgs;
};

#endif // COMMANDlINEPARSER_H