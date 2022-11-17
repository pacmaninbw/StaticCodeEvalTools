#include <vector>
#include <string>
#include "Executionctrlvalues.h"

ExecutionCtrlValues::ExecutionCtrlValues()
	:options{ ProgramOptions() },
	fileSpecTypes{ {} },
	filesToProcess{ {} }
{
}

void ExecutionCtrlValues::initFromEnvironmentVariables()
{
	options.initFromEnvironmentVars();
}


