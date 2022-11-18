#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "Executionctrlvalues.h"
#include "CommandLineParser.h"
#include "FileProcessor.h"
#include "ProgramOptions.h"
#include "UtilityTimer.h"

static void mainLoop(ExecutionCtrlValues& executionCtrl)
{
	std::string resultsToDisplay;

	ProgramOptions& options = executionCtrl.options;
	std::vector<std::string> filesToProcess = executionCtrl.filesToProcess;

	FileProcessor fileProcessor(filesToProcess, options);
	resultsToDisplay = fileProcessor.processAllFiles();
	// Yes we want to flush the standard output. All possible output has been
	// collected.
	std::cout << resultsToDisplay << std::endl;
}

int main(int argc, char* argv[])
{
	ExecutionCtrlValues executionCtrl;
	std::string versionString("1.0.0");

	std::locale::global(std::locale{""});
	std::clog.imbue(std::locale{});

	CommandLineParser cmdLineParser(argc, argv, versionString);

	try
	{
		executionCtrl.initFromEnvironmentVariables();
		if (cmdLineParser.parse(executionCtrl))
		{
			UtilityTimer stopWatch;
			mainLoop(executionCtrl);
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
