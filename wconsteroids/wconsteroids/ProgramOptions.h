#ifndef PROGRAM_OPTIONS_STRUCT_H
#define PROGRAM_OPTIONS_STRUCT_H

#include <string>
#include <string_view>
#include <unordered_map>

/*
 * This class is not generic except for the public functions. It is used
 * to contain the values of the switches/flags on the command line.
 */
class ProgramOptions {
public:
	ProgramOptions();
	/*
	 * Get any environment values and set them, this is an alternate to
	 * using the command line. Not currently used in this program.
	 */
	void initFromEnvironmentVars();
	/*
	 * SetDefaultOptionsWhenNoFlags(), make sure default values are set
	 * when necessary.
	 */
	void SetDefaultOptionsWhenNoFlags() noexcept;
	/*
	 * processArgument() is called by the command line parser to update 
	 * fields in this class. This hides the implementation from the
	 * command line parser.
	 */
	void processArgument(std::string_view arg) noexcept;
	ProgramOptions& operator=(const ProgramOptions& original)
	{
		blankLineCount = original.blankLineCount;
		byteCount = original.byteCount;
		charCount = original.charCount;
		codeCount = original.codeCount;
		commentCount = original.commentCount;
		lineCount = original.lineCount;
		maxLineWidth = original.maxLineWidth;
		percentages = original.percentages;
		whitespaceCount = original.whitespaceCount;
		wordCount = original.wordCount;
		enableExecutionTime = original.enableExecutionTime;
		recurseSubDirectories = original.recurseSubDirectories;
		version = original.version;
		programName = original.programName;

		return *this;
	}

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

	const std::unordered_map<std::string_view, bool&> doubleDashArgs{
		{ "--bytes", byteCount },
		{ "--chars", charCount },
		{ "--lines", lineCount },
		{ "--max-line-length", maxLineWidth },
		{ "--words", wordCount }
	};
	const std::unordered_map<char, bool&> singleDashArgs{
		{ 'c', byteCount },
		{ 'm', charCount },
		{ 'l', lineCount },
		{ 'L', maxLineWidth },
		{ 'w', wordCount }
	};
	/*
	 * Program name and program version should be included in any implementation.
	 */
	std::string_view version = "1.0.0";
	std::string programName = {};

protected:
	void printHelpMessage() const;
	void printVersion() const noexcept;
	void processSingleDashOptions(std::string_view currentArg);
	void processDoubleDashOptions(std::string_view currentArg);

private:
	bool useDefaultFlags = true;

#ifdef _DEBUG
public:
	void singleLine(std::string flag, bool flagValue);
	void debugPrint();
#endif // _DEBUG
};

#endif // PROGRAM_OPTIONS_STRUCT_H

