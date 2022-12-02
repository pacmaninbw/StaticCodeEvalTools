#ifndef REPORT_WRITER_H
#define REPORT_WRITER_H

/*
 * This class prints the output about the file statistics. It can also
 * return a formatted string of the output.
 */
#include <string>
#include <vector>

class ProgramOptions;
class FileStatistics;

class ReportWriter
{
public:
	// ExecutionCrtlValues is passed in so that the report writer know what
	// output to generate.
	ReportWriter(const ProgramOptions& progOptions)
		: options { progOptions }
	{
	}
	std::ostream& printResult(FileStatistics& resultsForOutput, std::ostream& os);
	std::string getResultText(FileStatistics& resultsForOutput) noexcept;
	std::ostream& printColumnHeadings(std::ostream& os) noexcept;
	// Returns 2 lines of properly formatted text
	std::string getColumnHeadingAsOneString() noexcept;

private:
	// The program options are necessary to know what to output.
	const ProgramOptions& options;
	std::string correctFileSpec(std::string fileSpec) noexcept;
};

#endif // REPORT_WRITER_H
