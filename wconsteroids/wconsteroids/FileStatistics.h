#ifndef	FILE_STATISTICS_H
#define FILE_STATISTICS_H

/*
 * Class to collect all the statistics about the code in a file.
 * 
 */
#include <string>

class FileStatistics
{
private:
	std::size_t totalLineCount = 0;
	std::size_t codeLineCount = 0;
	std::size_t commentLineCount = 0;
	std::size_t whiteSpaceCount = 0;
	std::size_t characterCount = 0;
	std::size_t wordCount = 0;
	std::size_t codeWithCommentCount = 0;
	std::size_t widestLine = 0;
	std::size_t blankLineCount = 0;
	std::string fileName = {};

public:
	FileStatistics();
	FileStatistics(std::string inFileName);
	void setFileName(std::string inFileName) { fileName = inFileName; }
	std::string getFileName() { return fileName; }
	void addTotals(FileStatistics &allFiles);
	void addToLineCount(std::size_t lineCount) { totalLineCount += lineCount; }
	void setToLineCount(std::size_t lineCount) { totalLineCount = lineCount; }
	void addToCharCount(std::size_t charCount) { characterCount += charCount; }
	void setCharCount(std::size_t charCount) { characterCount = charCount; }
	void addToWordCount(std::size_t wordCountUpdate) { wordCount += wordCountUpdate; }
	void setWordCount(std::size_t wordCountUpdate) { wordCount = wordCountUpdate; }
	void addToWhitespace(std::size_t wsUpdate) { whiteSpaceCount += wsUpdate; }
	// While the inline key word is only a recommendation, hopefully the increment
	// functions can be inline.
	inline void incrementTotalLines() { totalLineCount++; }
	std::size_t getTotalLines() const { return totalLineCount; }
	inline void incrementCodeLines() { codeLineCount++; }
	std::size_t getCodeLines() const { return codeLineCount; }
	inline void incrementCommentsLines() { commentLineCount++; }
	std::size_t getCommentLines() const { return commentLineCount; }
	inline void incrementWhitespace() { whiteSpaceCount++; }
	std::size_t getWhitespace() const { return whiteSpaceCount; }
	inline void incrementCharacter() { characterCount++; }
	std::size_t getCharacters() const { return characterCount; }
	inline void incrementWords() { wordCount++; }
	std::size_t getWords() const { return wordCount; }
	inline void incrementCodeWithComment() { codeWithCommentCount++; }
	std::size_t getCodeWithComment() const { return codeWithCommentCount; }
	inline void incrementBlankLines() { blankLineCount++; }
	std::size_t getBlankLines() { return blankLineCount; }
	void updateWidestLine(std::size_t width) {
		widestLine = (width > widestLine)? width : widestLine;
	}
	std::size_t getWidestLine() { return widestLine; }
	float getPerecentageOfCode() {
		return static_cast<float>(codeLineCount / totalLineCount);
	}
};

#endif // FILE_STATISTICS_H
