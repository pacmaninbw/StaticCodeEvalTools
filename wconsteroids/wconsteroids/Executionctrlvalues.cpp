#include <vector>
#include <string>
#include "Executionctrlvalues.h"

ExecutionCtrlValues::ExecutionCtrlValues()
{
}

void ExecutionCtrlValues::initFromEnvironmentVariables()
{
	options.initFromEnvironmentVars();
}


