#include <vector>
#include <string>
#include "Executionctrlvalues.h"

/*
 * The variables are initialized to the current default values of wc. 
 */
ExecutionCtrlValues::ExecutionCtrlValues()
{
	ProgramOptions* op = &this->options;
	// Set all options to false;
	memset(op, 0, sizeof(*op));
}

void ExecutionCtrlValues::initFromEnvironmentVariables()
{

}


