#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include <string>
#include <iostream>

#define FILE_STATISTICS_TEST_ID "A"
#define PROGRAM_OPTIONS_TEST_ID "B"
#define STATISTICS_COLLECTOR_TEST_ID "C"
#define REPORT_WRITER_TEST_ID "D"
#define FILE_PROCESSOR_TEST_ID "E"
#define CMD_LINE_FILE_EXTRACTOR_TEST_ID "F"
#define COMMAND_LINE_PARSER_TEST_ID "G"

extern std::string FindUnitTestDirectorRoot(void);

#endif // UNIT_TEST_H