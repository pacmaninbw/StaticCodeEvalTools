#include <chrono>
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
			std::chrono::time_point<std::chrono::system_clock> start, end;
			if (executionCtrl.options.enableExecutionTime)
			{
				start = std::chrono::system_clock::now();
			}
			mainLoop(executionCtrl);

			if (executionCtrl.options.enableExecutionTime)
			{
				end = std::chrono::system_clock::now();

				std::chrono::duration<double> elapsed_seconds = end - start;
				std::time_t end_time = std::chrono::system_clock::to_time_t(end);
				double ElapsedTimeForOutPut = elapsed_seconds.count();

				std::cout << "finished execution at " << std::ctime(&end_time)
					<< "elapsed time: " << ElapsedTimeForOutPut <<
					"\n" << "\n" << "\n";
			}

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