#ifndef EXECUTIONCONTROLVARIABLES_
#define EXECUTIONCONTROLVARIABLES_

/*
 * Storage for environment and commandline argument variables, also stores
 * list of files to process.
 */
#include <vector>
#include <string>
#include "ProgramOptions.h"

class ExecutionCtrlValues
{
public:
	ExecutionCtrlValues();
	~ExecutionCtrlValues() = default;
	void initFromEnvironmentVariables();
	ProgramOptions options;
	std::vector<std::string> fileSpecTypes;
	std::vector<std::string> filesToProcess;
};

#endif // EXECUTIONCONTROLVARIABLES_