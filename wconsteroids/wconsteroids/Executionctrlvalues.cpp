#include <vector>
#include <string>
#include "Executionctrlvalues.h"

/*
 * The variables are initialized to the current default values of wc. 
 */
ExecutionCtrlValues::ExecutionCtrlValues()
	: blankLineCount{ false }, byteCount{ true }, charCount{ false }, codeCount{ false },
	commentCount{ false }, lineCount{ true }, maxLineWidth{ false }, percentages{ false },
	whitespaceCount{ false }, wordCount{ true }, recurseSubDirectories{ false }
{

}

void ExecutionCtrlValues::initFromEnvironmentVariables()
{

}


