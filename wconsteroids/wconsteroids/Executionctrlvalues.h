#ifndef EXECUTION_CONTROL_VARIABLES_H
#define EXECUTION_CONTROL_VARIABLES_H

/*
 * Storage for environment and commandline argument variables, also stores
 * list of files to process. This class contains all the information to
 * execute the program after the command line has been processed.
 *
 * This class is fairly generic and can be reused by multiple programs that
 * process command line arguments.
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
	ProgramOptions options = {};
	std::vector<std::string> fileSpecTypes = {};
	std::vector<std::string> filesToProcess = {};
};

#endif // EXECUTION_CONTROL_VARIABLES_H
