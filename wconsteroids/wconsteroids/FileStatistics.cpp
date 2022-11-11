#include "FileStatistics.h"
#include <string>

FileStatistics::FileStatistics()
	: totalLineCount{ 0 },
	codeLineCount{ 0 },
	commentLineCount{ 0 },
	whiteSpaceCount{ 0 },
	wordCount{ 0 },
	characterCount{ 0 },
	codeWithCommentCount{ 0 },
	blankLineCount{ 0 },
	widestLine{ 0 }
{

}

FileStatistics::FileStatistics(std::string inFileName)
	: totalLineCount{ 0 },
	codeLineCount{ 0 },
	commentLineCount{ 0 },
	whiteSpaceCount{ 0 },
	wordCount{ 0 },
	characterCount{ 0 },
	codeWithCommentCount{ 0 },
	blankLineCount{ 0 },
	widestLine{ 0 },
	fileName{ inFileName }
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
