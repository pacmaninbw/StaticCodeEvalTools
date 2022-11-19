#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "Executionctrlvalues.h"
#include "CommandLineParser.h"
#include "FileProcessor.h"
#include "ProgramOptions.h"
#include "UtilityTimer.h"

int main(int argc, char* argv[])
{
	ExecutionCtrlValues executionCtrl;

	std::locale::global(std::locale{""});
	std::clog.imbue(std::locale{});

	CommandLineParser cmdLineParser(argc, argv);

	try
	{
		executionCtrl.initFromEnvironmentVariables();
		if (cmdLineParser.parse(executionCtrl))
		{
			UtilityTimer stopWatch;
            std::cout << processAllFiles(executionCtrl.filesToProcess, executionCtrl.options)
				<< std::endl;
			if (executionCtrl.options.enableExecutionTime)
			{
				stopWatch.stopTimerAndReport(
					" processing and reporting input files ");
			}
		}
	}

	catch (const std::exception &ex)
	{
		std::cerr << "Error: " << ex.what() << "\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
