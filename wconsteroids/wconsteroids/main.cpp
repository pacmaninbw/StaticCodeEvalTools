#include <iostream>
#include <string>
#include "cmdlineoutputtypes.h"
#include "CommandLineParser.h"
#include "FileStatistics.h"
#include "ReportWriter.h"

static void mainLoop(CmdLineOutputTypes outputCtrl, FileStatistics allFiles)
{

}

int main(int argc, char* argv[])
{
	int exit_status = EXIT_SUCCESS;

	CmdLineOutputTypes outputCtrl = CmdLineOutputTypes::Default;

	FileStatistics allFiles;

	try
	{
		CommandLineParser cmdLineParser(argc, argv);
		cmdLineParser.parse();
		outputCtrl = cmdLineParser.getOutputTypes();
	}

	catch (std::exception ex)
	{
		std::cerr << "Error: " << ex.what() << "\n";
		exit_status = EXIT_FAILURE;
	}

	return exit_status;
}