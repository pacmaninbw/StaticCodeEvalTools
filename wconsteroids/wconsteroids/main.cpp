#include <iostream>
#include <string>
#include "cmdlineoutputtypes.h"
#include "CommandLineParser.h"
#include "FileStatistics.h"
#include "ReportWriter.h"

static void mainLoop(CommandLineParser &cmdLineParser)
{
	CmdLineOutputTypes outputCtrl = CmdLineOutputTypes::Default;
	FileStatistics allFiles;

	outputCtrl = cmdLineParser.getOutputTypes();
	std::vector<std::string> filesToProcess = cmdLineParser.getFileList();

}

int main(int argc, char* argv[])
{
	int exit_status = EXIT_SUCCESS;

	try
	{
		CommandLineParser cmdLineParser(argc, argv);
		if (cmdLineParser.parse())
		{
			mainLoop(cmdLineParser);
		}
	}

	catch (std::exception ex)
	{
		std::cerr << "Error: " << ex.what() << "\n";
		exit_status = EXIT_FAILURE;
	}

	return exit_status;
}