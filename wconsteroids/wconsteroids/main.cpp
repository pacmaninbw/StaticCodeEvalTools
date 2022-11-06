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

static char** makeFakeArgs()
{
	char** fakeArgs = new char* [4];
	std::string arg1("-Llcw");
	std::string arg2("-R");
	std::string arg3("*.cpp");
	std::string arg4("*.h");

	fakeArgs[0] = new char[arg1.length() + 1];
	strcpy(fakeArgs[0], arg1.c_str());

	fakeArgs[1] = new char[arg2.length() + 1];
	strcpy(fakeArgs[1], arg2.c_str());

	fakeArgs[2] = new char[arg3.length() + 1];
	strcpy(fakeArgs[2], arg3.c_str());

	fakeArgs[3] = new char[arg4.length() + 1];
	strcpy(fakeArgs[3], arg4.c_str());

	return fakeArgs;
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

	char** FAKEARGV = makeFakeArgs();
	int FAKEARGC = sizeof(FAKEARGV) / sizeof(*FAKEARGV);
	CommandLineParser cmdLineParser(4, FAKEARGV, versionString);

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