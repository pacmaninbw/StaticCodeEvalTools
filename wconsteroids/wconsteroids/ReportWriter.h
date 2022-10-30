#ifndef REPORTWRITER_H
#define REPORTWRITER_H

#include <string>
#include "cmdlineoutputtypes.h"
#include "FileStatistics.h"

class ReportWriter
{
public:
	ReportWriter(CmdLineOutputTypes inOutputTypes= CmdLineOutputTypes::Default)
		:outputTypes { inOutputTypes } {}
	void printResult(FileStatistics& resultsForOutput);
	std::string getResultText(FileStatistics& resultsForOutput);

private:
	CmdLineOutputTypes outputTypes;
};

#endif // REPORTWRITER_H
