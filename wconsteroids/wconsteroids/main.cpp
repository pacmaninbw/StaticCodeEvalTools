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
	int exit_status = EXIT_SUCCESS;
	ExecutionCtrlValues executionCtrl;
	std::string versionString("1.0.0");

	CommandLineParser cmdLineParser(argc, argv, versionString);

	try
	{
		executionCtrl.initFromEnvironmentVariables();
		if (cmdLineParser.parse(executionCtrl))
		{
			UtilityTimer stopWatch;
			if (executionCtrl.options.enableExecutionTime)
			{
				stopWatch.startTimer();
			}
			mainLoop(executionCtrl);
			if (executionCtrl.options.enableExecutionTime)
			{
				stopWatch.stopTimerAndReport(
					" processing and reporting input files ");
			}
		}
	}

	catch (const ShowHelpMessage&)
	{
		cmdLineParser.printHelpMessage();
		cmdLineParser.printVersion();
	}
	catch (const showVersions&)
	{
		cmdLineParser.printVersion();
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Error: " << ex.what() << "\n";
		exit_status = EXIT_FAILURE;
	}

	return exit_status;
}
