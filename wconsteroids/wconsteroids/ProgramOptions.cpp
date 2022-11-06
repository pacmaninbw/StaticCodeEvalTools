#include <iostream>
#include "ProgramOptions.h"

#ifdef _DEBUG
void ProgramOptions::singleLine(std::string flag, bool flagValue)
{
	std::cout << "Flag " << flag << ": Flag value " <<
		((flagValue) ? "True" : "False") << "\n";
}

void ProgramOptions::debugPrint()
{
	singleLine("blankLineCount", blankLineCount);
	singleLine("byteCount", byteCount);
	singleLine("charCount", charCount);
	singleLine("codeCount", codeCount);
	singleLine("commentCount", commentCount);
	singleLine("lineCount", lineCount);
	singleLine("maxLineWidth", maxLineWidth);
	singleLine("percentages", percentages);
	singleLine("whitespaceCount", whitespaceCount);
	singleLine("wordCount", wordCount);
	singleLine("recurseSubDirectories", recurseSubDirectories);
}
#endif
