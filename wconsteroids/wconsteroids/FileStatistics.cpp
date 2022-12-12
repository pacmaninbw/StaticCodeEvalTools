#include <doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include "FileStatistics.h"
#include <string>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

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

bool FileStatistics::areResultsEmpty()
{
	return
		totalLineCount == 0 &&
		codeLineCount == 0 &&
		commentLineCount == 0 &&
		whiteSpaceCount == 0 &&
		wordCount == 0 &&
		characterCount == 0 &&
		codeWithCommentCount == 0 &&
		blankLineCount == 0 &&
		widestLine == 0;
}
