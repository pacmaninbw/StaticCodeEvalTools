#ifndef STATISTICS_COLLECTOR_H
#define STATISTICS_COLLECTOR_H

#include <iterator>
#include <string_view>

class FileStatistics;

class StatisticsCollector
{
public:
	StatisticsCollector(FileStatistics& fileStats);
	void analyzeBuffer(std::string_view inputBuffer) noexcept;

protected:
	void updateWidestLine(std::string_view::const_iterator& currentChar,
		std::string_view::const_iterator end) noexcept;
	void countWordsAndWhiteSpace(std::string_view inputBuffer) noexcept;

private:
	FileStatistics& fileStatistics;
};

#endif // STATISTICS_COLLECTOR_H
