#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "CommandLineParser.h"
#include "CmdLineFileExtractor.h"
#include "Executionctrlvalues.h"
#include "SpecialExceptions.h"
#include "UtilityTimer.h"

/*
 * Begin public interface.
 */
CommandLineParser::CommandLineParser(int argc, char* argv[],
	std::string progVersion)
	: program_name{ find_prog_name(argv[0]) },
	args(argv + 1, argv + argc),
	version{ std::move(progVersion) },
	programName{ argv[0] },
	options{ ProgramOptions() },
	doubleDashArgs{
		{ "--bytes", options.byteCount },
		{ "--chars", options.charCount },
		{ "--lines", options.lineCount },
		{ "--max-line-length", options.maxLineWidth },
		{ "--words", options.wordCount }
	},
	singleDashArgs{
		{ 'c', options.byteCount },
		{ 'm', options.charCount },
		{ 'l', options.lineCount },
		{ 'L', options.maxLineWidth },
		{ 'w', options.wordCount }
	},
	NotFlagsArgs{ {} },
	useDefaultFlags{ true }
{
	std::filesystem::path programPath(argv[0]);
	programName = std::move(programPath.filename().string());

}

bool CommandLineParser::parse(ExecutionCtrlValues& execVars)
{
	UtilityTimer stopWatch;
	// There is no way to determine if -t has been used at this point
	// so start the timer anyway.
	stopWatch.startTimer();

	bool hasFiles = false;

	extractAllArguments();
	if (useDefaultFlags)
	{
		SetDefaultOptionsWhenNoFlags();
	}

	findAllFilesToProcess(execVars);
	execVars.options = options;

	if (options.enableExecutionTime)
	{
		stopWatch.stopTimerAndReport("command line parsing at ");
	}

	return hasFiles = execVars.filesToProcess.size() != 0;
}

void CommandLineParser::printHelpMessage() const
{
	std::cout << "\n" << programName <<
		" file name or file type specification (*.ext)\n"
		"Otions:\n"
		"\t-c, --bytes print the byte counts\n"
		"\t-m, --chars print the character counts\n"
		"\t-l, --lines print the newline counts\n"
		"\t-t, --time-execution print the execution time of the program\n"
		"\t-L, --max-line-length print the length of the longest line\n"
		"\t-w, --words print the word counts\n"
		"\t--help display this help and exit\n"
		"\t--version output version information and exit\n"
		"\t-R, --subdirectories all files in the"
		" directory as well as sub directories\n"
		"\tBy default the -c -l and -w flags are set, setting any"
		" flag requires all flags you want to be set.\n";
	printVersion();
}

void CommandLineParser::printVersion() const noexcept
{
	std::cout << programName << ": version: " << version << "\n"
		"Packaged by Chernick Consulting\n"
		"License GPLv3+: GNU GPL version 3 or later"
		" <http://gnu.org/licenses/gpl.html>.\n"
		"This is free software : you are free to change and redistribute it.\n"
		"\tThere is NO WARRANTY, to the extent permitted by law.\n"
		"\nWritten by Paul A. Chernick\n";
	// flush the buffer to make sure the entire message is visible
	std::cout << std::flush;
	exit(EXIT_SUCCESS);
}

void CommandLineParser::findAllFilesToProcess(ExecutionCtrlValues& execVars)
{
	bool searchSubDirs = options.recurseSubDirectories;
	CmdLineFileExtractor fileExtractor(NotFlagsArgs, searchSubDirs);
	fileExtractor.findAllRequiredFiles();
	execVars.filesToProcess = fileExtractor.getFileList();
	execVars.fileSpecTypes = fileExtractor.getFileTypeList();
}

void CommandLineParser::extractAllArguments()
{
	// start after the program name.
	for (std::size_t i = 0; i < args.size(); i++)
	{
		if (args[i][0] == '-')
		{
			if (args[i][1] == '-')
			{
				processDoubleDashOptions(args[i]);
			}
			else
			{
				processSingleDashOptions(args[i]);
			}
		}
		else
		{
			NotFlagsArgs.push_back(std::move(args[i]));
		}
	}
}

/*
 * Flags starting with -- are full strings that need to be processed
 * as strings.
 */
void CommandLineParser::processDoubleDashOptions(std::string_view currentArg)
{
	auto flag = doubleDashArgs.find(currentArg);
	if (flag != doubleDashArgs.end())
	{
		(*flag).second = true;
		useDefaultFlags = false;
		return;
	}

	// The following switches require alternate handling
	if (currentArg == "--subdirectories")
	{
		// Since this is not a column switch it does not affect the default 
		options.recurseSubDirectories = true;
		return;
	}

	if (currentArg == "--time-execution")
	{
		// Since this is not a column switch it does not affect the default 
		options.enableExecutionTime = true;
		return;
	}

	if (currentArg == "--help")
	{
		// calls printVersion.
		printHelpMessage();
	}

	if (currentArg == "--version")
	{
		// calls exit after printing version
		printVersion();
	}

	std::cerr << "Unknown flag: " << currentArg << "\n";
}

/*
 * Each character needs to be processed independently.
 */
void CommandLineParser::processSingleDashOptions(std::string_view currentArg)
{
	for (std::size_t i = 1; i < currentArg.size(); i++)
	{
		auto thisOption = singleDashArgs.find(currentArg[i]);
		if (thisOption != singleDashArgs.end())
		{
			(*thisOption).second = true;
			useDefaultFlags = false;
		}
		else
		{
			switch (currentArg[i])
			{
			case 'R':
				// Since this is not a column switch it does not affect the
				// default  
				options.recurseSubDirectories = true;
				continue;
			case 't':
				// Since this is not a column switch it does not affect the
				// default  
				options.enableExecutionTime = true;
				continue;
			default:
				std::cerr << "Unknown flag: " << currentArg[i] << "\n";
				continue;
			}
		}
	}
}

void CommandLineParser::SetDefaultOptionsWhenNoFlags()
{
	// Based on the default functionality of the wc program.
	options.byteCount = true;
	options.wordCount = true;
	options.lineCount = true;
}

