#ifndef PROGRAM_OPTIONS_STRUCT_H
#define PROGRAM_OPTIONS_STRUCT_H

/*
 * This class is not generic except for the public functions. It is used
 * to contain the the values of the switches/flags on the command line.
 */
class ProgramOptions {
public:
	// Output control variables
	bool blankLineCount = false;
	bool byteCount = false;
	bool charCount = false;
	bool codeCount = false;
	bool commentCount = false;
	bool lineCount = false;
	bool maxLineWidth = false;
	bool percentages = false;
	bool whitespaceCount = false;
	bool wordCount = false;
	bool enableExecutionTime = false;
	// input control variables
	bool recurseSubDirectories = false;

	ProgramOptions()
	{

	}
	void initFromEnvironmentVars()
	{

	}

#ifdef _DEBUG
	void singleLine(std::string flag, bool flagValue);
	void debugPrint();
#endif // _DEBUG
};

#endif // PROGRAM_oPTIONS_STRUCT_H

