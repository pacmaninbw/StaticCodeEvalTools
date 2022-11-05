#include <iostream>
#include <string>
#include "Executionctrlvalues.h"
#include "CommandLineParser.h"
#include "FileStatistics.h"
#include "ReportWriter.h"

#ifdef _DEBUG
static void debugMainLoop(ExecutionCtrlValues& executionCtrl)
{
	ProgramOptions debugOptions = executionCtrl.options;
	debugOptions.debugPrint();

	std::cout << "\nFile specifications:\n";
	for (auto fileSpec : executionCtrl.fileSpecTypes)
	{
		std::cout << "\t" << fileSpec << "\n";
	}

	std::cout << "\nFile names:\n";
	for (auto fileName : executionCtrl.filesToProcess)
	{
		std::cout << "\t" << fileName << "\n";
	}
}
#endif

static void mainLoop(ExecutionCtrlValues& executionCtrl)
{
	FileStatistics allFiles;

#ifdef _DEBUG
	debugMainLoop(executionCtrl);
#endif
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