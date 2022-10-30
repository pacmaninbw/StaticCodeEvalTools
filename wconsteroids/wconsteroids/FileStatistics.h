#ifndef	FILESTATISTICS_H
#define FILESTATISTICS_H

#include <string>

class FileStatistics
{
private:
	size_t totalLineCount;
	size_t codeLineCount;
	size_t commentLineCount;
	size_t whiteSpaceCount;
	size_t characterCount;
	size_t wordCount;
	size_t codeWithCommentCount;
	std::string fileName; 

public:
	FileStatistics();
	FileStatistics(std::string inFileName);
	void setFileName(std::string inFileName) { fileName = inFileName; }
	void addTotals(FileStatistics &allFiles);
	void incrementTotalLines() { totalLineCount++; }
	size_t getTotalLines() const { return totalLineCount; }
	void incrementCodeLines() { codeLineCount++; }
	size_t getCodeLines() const { return codeLineCount; }
	void incrementCommentsLines() { commentLineCount++; }
	size_t getCommentLines() const { return commentLineCount; }
	void incrementWhitespace() { whiteSpaceCount++; }
	size_t getWhitespace() const { return whiteSpaceCount; }
	void incrementCharacter() { characterCount++; }
	size_t getCharacters() const { return characterCount; }
	void incrementWords() { wordCount++; }
	size_t getWords() const { return wordCount; }
	void incrementCodeWithComment() { codeWithCommentCount++; }
	size_t getCodeWithComment() const { return codeWithCommentCount; }
};

#endif // FILESTATISTICS_H