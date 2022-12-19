#ifndef EXECUTION_CONTROL_VARIABLES_H
#define EXECUTION_CONTROL_VARIABLES_H

/*
 * Storage for environment and commandline argument variables, also stores
 * list of files to process. This class contains all the information to
 * execute the program after the command line has been processed.
 *
 * This struct is fairly generic and can be reused by multiple programs that
 * process command line arguments.
 * 
 * The ProgramOptions class contains all information that is dependent on 
 * program implementation.
 */
#include <vector>
#include <string>
#include "ProgramOptions.h"

struct ExecutionCtrlValues
{
	ProgramOptions options = {};
	std::vector<std::string> filesToProcess = {};

	void initFromEnvironmentVariables()
	{
		options.initFromEnvironmentVars();
	}
};

#endif // EXECUTION_CONTROL_VARIABLES_H
