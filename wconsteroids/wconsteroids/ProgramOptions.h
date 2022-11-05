#ifndef PROGRAM_OPTIONS_STRUCT_H
#define PROGRAM_OPTIONS_STRUCT_H

class ProgramOptions {
public:
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
	ProgramOptions()
		: blankLineCount{ false },
		byteCount{ false },
		charCount{ false },
		codeCount{ false },
		commentCount{ false },
		lineCount{ false },
		maxLineWidth{ false },
		percentages{ false },
		whitespaceCount{ false },
		wordCount{ false },
		recurseSubDirectories{ false }
	{

	}

#ifdef _DEBUG
	void singleLine(std::string flag, bool flagValue);
	void debugPrint();
#endif // _DEBUG
};

#endif // PROGRAM_oPTIONS_STRUCT_H

