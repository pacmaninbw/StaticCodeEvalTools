#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <algorithm>
#include <filesystem>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include "FileStatistics.h"
#include "ProgramOptions.h"
#include "ReportWriter.h"
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

struct Column {
	bool ProgramOptions::* flag;
	std::size_t (FileStatistics::*count)() const;
	std::string header[2];
	int width;

	Column(bool ProgramOptions::* flag,
	       std::size_t (FileStatistics::* count)() const,
	       std::string first, std::string second, std::size_t width = 0)
		: flag { flag },
		  count { count },
		  header { std::move(first), std::move(second) },
		  width { static_cast<int>(std::max({header[0].size(), header[1].size(), width})) }
	{}
};

static const Column columns[] =
{
	{ &ProgramOptions::lineCount,
	  &FileStatistics::getTotalLines,
	  "Lines", "of Text", 6 },
	{ &ProgramOptions::wordCount,
	  &FileStatistics::getWords,
	  "Words", "", 7 },
	{ &ProgramOptions::byteCount,
	  &FileStatistics::getCharacters,
	  "Bytes", "", 8 },
	{ &ProgramOptions::charCount,
	  &FileStatistics::getCharacters,
	  "Characters", "", 8 },
	{ &ProgramOptions::maxLineWidth,
	  &FileStatistics::getWidestLine,
	  "Length of", "Longest Line", 5 },
	{ &ProgramOptions::codeCount,
	  &FileStatistics::getCodeLines,
	  "Lines", "of Code", 6 },
	{ &ProgramOptions::commentCount,
	  &FileStatistics::getCommentLines,
	  "Lines of", "Comments", 6 },
	{ &ProgramOptions::percentages,
	  &FileStatistics::getPerecentageOfCode,
	  "Percentage of", "Lines of Code", 2 },
	{ &ProgramOptions::whitespaceCount,
	  &FileStatistics::getWhitespace,
	  "Whitespace", "Characters", 8 },
	{ &ProgramOptions::blankLineCount,
	  &FileStatistics::getBlankLines,
	  "Blank", "Lines", 6 },
};

std::ostream& ReportWriter::printResult(FileStatistics& resultsForOutput, std::ostream& os)
{
	for (auto const& col: columns) {
		if (options.*(col.flag)) {
			os << std::setw(col.width)
			   << (resultsForOutput.*col.count)()
			   << '\t';
		}
	}

	os << correctFileSpec(resultsForOutput.getFileName())
	   << '\n';

	return os;
}

std::string ReportWriter::getResultText(FileStatistics& resultsForOutput) noexcept
{
	std::ostringstream os;
	printResult(resultsForOutput, os);
	return os.str();
}

std::string ReportWriter::getColumnHeadingAsOneString() noexcept
{
	std::ostringstream os;
	printColumnHeadings(os);
	return os.str();
}

std::ostream& ReportWriter::printColumnHeadings(std::ostream& os) noexcept
{
	for (int i = 0;  i <= 1;  ++i) {
		for (auto const& col: columns) {
			if (options.*(col.flag)) {
				os << std::setw(col.width)
				   << (col.header[i])
				   << '\t';
			}
		}
		os << '\n';
	}

	return os;
}

std::string ReportWriter::correctFileSpec(std::string fileSpec) noexcept
{
	// When using the -R switch there may be multiple files in the tree with
	// the same name, so use the full file spec in that case.
	return (options.recurseSubDirectories)? fileSpec:
		std::filesystem::path{fileSpec}.filename().string();
}
