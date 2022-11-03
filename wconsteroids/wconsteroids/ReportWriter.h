#ifndef REPORTWRITER_H
#define REPORTWRITER_H

/*
 * This class prints the output about the file statistics. It can also
 * return a formated string of the output.
 */
#include <memory>
#include <string>	// std::vector included by string
#include "Executionctrlvalues.h"
#include "FileStatistics.h"

class ReportWriter
{
public:
	// ExecutionCrtlValues is passed in so that the report writer know what
	// output to generate.
	ReportWriter(ExecutionCtrlValues& executionctrl)
	{
		executionCtrl = std::make_shared<ExecutionCtrlValues>(executionctrl);
	}
	void printResult(FileStatistics& resultsForOutput);
	std::string getResultText(FileStatistics& resultsForOutput);
	void printColumnHedings();
	// Returns 2 lines of properly formated text
	std::vector<std::string> getColumneHeadingsText();

private:
	std::shared_ptr <ExecutionCtrlValues> executionCtrl;

};

#endif // REPORTWRITER_H
