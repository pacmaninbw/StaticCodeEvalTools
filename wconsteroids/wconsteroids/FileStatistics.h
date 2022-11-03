#ifndef	FILESTATISTICS_H
#define FILESTATISTICS_H

/*
 * Class to collect all the statistics about the code in a file.
 * 
 */
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
	size_t widestLine;
	size_t blankLineCount;
	std::string fileName; 

public:
	FileStatistics();
	FileStatistics(std::string inFileName);
	void setFileName(std::string inFileName) { fileName = inFileName; }
	std::string getFileName() { return fileName; }
	void addTotals(FileStatistics &allFiles);
	void addToLineCount(size_t lineCount) { totalLineCount += lineCount; }
	void addToCharCount(size_t charCount) { characterCount += charCount; }
	void addToWordCount(size_t wordCountUpdate) { wordCount += wordCountUpdate; }
	// While the inline key word is only a recommendation, hopefully the increment
	// functions can be inline.
	inline void incrementTotalLines() { totalLineCount++; }
	size_t getTotalLines() const { return totalLineCount; }
	inline void incrementCodeLines() { codeLineCount++; }
	size_t getCodeLines() const { return codeLineCount; }
	inline void incrementCommentsLines() { commentLineCount++; }
	size_t getCommentLines() const { return commentLineCount; }
	inline void incrementWhitespace() { whiteSpaceCount++; }
	size_t getWhitespace() const { return whiteSpaceCount; }
	inline void incrementCharacter() { characterCount++; }
	size_t getCharacters() const { return characterCount; }
	inline void incrementWords() { wordCount++; }
	size_t getWords() const { return wordCount; }
	inline void incrementCodeWithComment() { codeWithCommentCount++; }
	size_t getCodeWithComment() const { return codeWithCommentCount; }
	inline void incrementBlankLines() { blankLineCount++; }
	size_t getBlankLines() { return blankLineCount; }
	void updateWidestLine(size_t width) {
		widestLine = (width > widestLine)? width : widestLine;
	}
	size_t getWidestLine() { return widestLine; }
	float getPerecentageOfCode() {
		return static_cast<float>(codeLineCount / totalLineCount);
	}
};

#endif // FILESTATISTICS_H