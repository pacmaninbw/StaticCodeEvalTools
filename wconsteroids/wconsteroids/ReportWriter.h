#ifndef REPORTWRITER_H
#define REPORTWRITER_H

/*
 * This class prints the output about the file statistics. It can also
 * return a formated string of the output.
 */
#include <memory>
#include <string>	// std::vector included by string
#include "ProgramOptions.h"
#include "FileStatistics.h"

class ReportWriter
{
public:
	// ExecutionCrtlValues is passed in so that the report writer know what
	// output to generate.
	ReportWriter(ProgramOptions& progOptions)
	{
		options = std::make_shared<ProgramOptions>(progOptions);
	}
	void printResult(FileStatistics& resultsForOutput);
	std::string getResultText(FileStatistics& resultsForOutput);
	void printColumnHeadings();
	// Returns 2 lines of properly formated text
	std::vector<std::string> getColumneHeadingsText();

private:
	// The program options are necessary to know what to outout.
	std::shared_ptr <ProgramOptions> options;

};

#endif // REPORTWRITER_H
