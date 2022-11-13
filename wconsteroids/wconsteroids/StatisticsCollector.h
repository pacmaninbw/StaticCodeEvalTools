#ifndef STATISTICS_COLLECTOR_H
#define STATISTICS_COLLECTOR_H

#include <iterator>
#include <string>
#include <vector>
#include "FileStatistics.h"

class StatisticsCollector
{
public:
	StatisticsCollector(FileStatistics& fileStats);
	void analyzeBuffer(std::string inputbuffer) noexcept;

protected:
	void updateWidestLine(std::string::iterator& currentChar,
		std::string::iterator end) noexcept;
	void lineWidth(std::string line) noexcept;
	void countWordsAndWhiteSpace(std::string& inputBuffer) noexcept;

private:
	FileStatistics& fileStatistics;
};

#endif // STATISTICS_COLLECTOR_H