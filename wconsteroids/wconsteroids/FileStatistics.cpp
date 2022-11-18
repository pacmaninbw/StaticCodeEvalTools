#include "FileStatistics.h"
#include <string>

FileStatistics::FileStatistics() = default;

FileStatistics::FileStatistics(std::string inFileName)
	: fileName{ std::move(inFileName) }
{

}

void FileStatistics::addTotals(FileStatistics &allFiles)
{
	allFiles.totalLineCount += totalLineCount;
	allFiles.codeLineCount += codeLineCount;
	allFiles.commentLineCount += commentLineCount;
	allFiles.whiteSpaceCount += whiteSpaceCount;
	allFiles.wordCount += wordCount;
	allFiles.characterCount += characterCount;
	allFiles.codeWithCommentCount += codeWithCommentCount;
	allFiles.blankLineCount += blankLineCount;
	allFiles.updateWidestLine(widestLine);
}
