#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <iostream>
#include "Executionctrlvalues.h"
#include "CmdLineFileExtractor.h"
#include "CommandLineParser.h"
#include "FileProcessor.h"
#include "FileStatistics.h"
#include "ProgramOptions.h"
#include "ReportWriter.h"
#include "StatisticsCollector.h"
#include "UtilityTimer.h"
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END