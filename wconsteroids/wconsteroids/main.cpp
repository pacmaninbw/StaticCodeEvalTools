#include <iostream>
#include <string>
#include "Executionctrlvalues.h"
#include "CommandLineParser.h"
#include "FileStatistics.h"
#include "FileProcessor.h"
#include "ProgramOptions.h"
#include "ReportWriter.h"

static void mainLoop(ExecutionCtrlValues& executionCtrl)
{
	FileStatistics allFiles;


	ProgramOptions& options = executionCtrl.options;
	std::vector<std::string> fileToProcess = executionCtrl.filesToProcess;

	ReportWriter TotalReporter(options);
	TotalReporter.printColumnHeadings();

	for (auto currentFile : fileToProcess)
	{
		try
		{
			FileProcessor fileProcessor(currentFile, options);
			if (fileProcessor.processFile())
			{
				fileProcessor.mergeStatistics(allFiles);
			}
		}
		catch (std::runtime_error re)
		{
			std::cerr << re.what() << "\n";
		}
	}
	
	TotalReporter.printColumnHeadings();
	TotalReporter.printResult(allFiles);
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
			mainLoop(executionCtrl);
		}
	}

	catch (HelpMe helpMe)
	{
		cmdLineParser.printHelpMessage();
	}
	catch (showVersions sv)
	{
		cmdLineParser.printVersion();
	}
	catch (std::exception ex)
	{
		std::cerr << "Error: " << ex.what() << "\n";
		exit_status = EXIT_FAILURE;
	}

	return exit_status;
}