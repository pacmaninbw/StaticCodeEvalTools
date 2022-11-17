#ifndef REPORT_WRITER_H
#define REPORT_WRITER_H

/*
 * This class prints the output about the file statistics. It can also
 * return a formated string of the output.
 */
#include <memory>
#include <string>
#include <vector>
#include "ProgramOptions.h"
#include "FileStatistics.h"

class ReportWriter
{
public:
	// ExecutionCrtlValues is passed in so that the report writer know what
	// output to generate.
	ReportWriter(ProgramOptions& progOptions)
		: options { std::make_shared<ProgramOptions>(progOptions) }
	{
	}
	void printResult(FileStatistics& resultsForOutput) noexcept;
	std::string getResultText(FileStatistics& resultsForOutput) noexcept;
	void printColumnHeadings() noexcept;
	// Returns 2 lines of properly formated text
	std::vector<std::string> getColumneHeadingsText() noexcept;
	std::string getColumnHeadingAsOneString() noexcept;

private:
	// The program options are necessary to know what to outout.
	std::shared_ptr <ProgramOptions> options;
	std::string correctFileSpec(std::string fileSpec) noexcept;
};

#endif // REPORT_WRITER_H
