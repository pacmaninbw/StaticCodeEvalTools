#include <iostream>
#include <string>
#include "Executionctrlvalues.h"
#include "CommandLineParser.h"
#include "FileStatistics.h"
#include "ReportWriter.h"

static void mainLoop(ExecutionCtrlValues& executionCtrl)
{
	FileStatistics allFiles;


}

int main(int argc, char* argv[])
{
	int exit_status = EXIT_SUCCESS;
	ExecutionCtrlValues executionCtrl;
	std::string versionString("1.0.0");

	try
	{
		executionCtrl.initFromEnvironmentVariables();
		CommandLineParser cmdLineParser(argc, argv, versionString);
		if (cmdLineParser.parse(executionCtrl))
		{
			mainLoop(executionCtrl);
		}
	}

	catch (std::exception ex)
	{
		std::cerr << "Error: " << ex.what() << "\n";
		exit_status = EXIT_FAILURE;
	}

	return exit_status;
}