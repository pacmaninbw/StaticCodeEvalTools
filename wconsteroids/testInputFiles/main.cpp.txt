#include <cstdlib>
#include <iostream>
#include "Executionctrlvalues.h"
#include "CommandLineParser.h"
#include "FileProcessor.h"
#include "UtilityTimer.h"

int main(int argc, char* argv[])
{
	std::locale::global(std::locale{""});
	std::clog.imbue(std::locale{});

	try
	{
		ExecutionCtrlValues executionCtrl;
		CommandLineParser cmdLineParser(argc, argv);

		if (cmdLineParser.parse(executionCtrl))
		{
			UtilityTimer stopWatch;
            std::cout << processAllFiles(executionCtrl) << std::endl;
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
