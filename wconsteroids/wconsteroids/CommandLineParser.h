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
	void initHelpMessage();
	void findAllFilesToProcess(ExecutionCtrlValues& execVars);
	unsigned int extractAllArguments();

private:
	char** args;
	int argCount;
	std::string version;
	ProgramOptions options;
	std::unordered_map<std::string, bool&> doubleDashArgs;
	std::unordered_map<char, bool&> singleDashArgs;
	std::vector<std::string> helpMessage;
	std::vector<std::string> NotFlags;
};

#endif // COMMANDlINEPARSER_H