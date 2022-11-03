#ifndef EXECUTIONCONTROLVARIABLES_
#define EXECUTIONCONTROLVARIABLES_

/*
 * Storage for environment and commandline argument variables, also stores
 * list of files to process.
 */
#include <vector>
#include <string>

class ExecutionCtrlValues
{
public:
	ExecutionCtrlValues();
	~ExecutionCtrlValues() = default;
	void initFromEnvironmentVariables();
	// Output control variables
	bool blankLineCount;
	bool byteCount;
	bool charCount;
	bool codeCount;
	bool commentCount;
	bool lineCount;
	bool maxLineWidth;
	bool percentages;
	bool whitespaceCount;
	bool wordCount;
	// input control variables
	bool recurseSubDirectories;
	std::vector<std::string> fileSpecTypes;
	std::vector<std::string> filesToProcess;
};

#endif // EXECUTIONCONTROLVARIABLES_