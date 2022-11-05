#include <cstring>
#include <vector>
#include <string>
#include "Executionctrlvalues.h"

ExecutionCtrlValues::ExecutionCtrlValues()
{
	ProgramOptions* op = &this->options;
	// Set all options to false;
	memset(op, 0, sizeof(*op));
}

void ExecutionCtrlValues::initFromEnvironmentVariables()
{

}


