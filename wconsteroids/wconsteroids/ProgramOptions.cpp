#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include "ProgramOptions.h"

/*
 * Public interfaces.
 */
ProgramOptions::ProgramOptions()
{

}
void ProgramOptions::initFromEnvironmentVars()
{

}

void ProgramOptions::SetDefaultOptionsWhenNoFlags() noexcept
{
	if (!useDefaultFlags)
	{
		return;
	}
	// Based on the default functionality of the wc program.
	byteCount = true;
	wordCount = true;
	lineCount = true;
}

void ProgramOptions::processArgument(std::string_view arg) noexcept
{
	if (arg[1] == '-')
	{
		processDoubleDashOptions(arg);
	}
	else
	{
		processSingleDashOptions(arg);
	}
}

/*
 * Protected and private functions.
 */

/*
 * Flags starting with -- are full strings that need to be processed
 * as strings.
 */
void ProgramOptions::processDoubleDashOptions(std::string_view currentArg)
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
		recurseSubDirectories = true;
		return;
	}

	if (currentArg == "--time-execution")
	{
		// Since this is not a column switch it does not affect the default 
		enableExecutionTime = true;
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
void ProgramOptions::processSingleDashOptions(std::string_view currentArg)
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
				recurseSubDirectories = true;
				continue;
			case 't':
				// Since this is not a column switch it does not affect the
				// default  
				enableExecutionTime = true;
				continue;
			default:
				std::cerr << "Unknown flag: " << currentArg[i] << "\n";
				continue;
			}
		}
	}
}

void ProgramOptions::printHelpMessage() const
{
	std::cout << "\n" << programName <<
		" [Options] file name or file type specification (*.ext)\n"
		"Options:\n"
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
		" flag requires all flags you want to be set.\n\n";
	printVersion();
}

void ProgramOptions::printVersion() const noexcept
{
	std::cout << programName << ": version: " << version << "\n"
		"Packaged by Chernick Consulting\n"
		"License GPLv3+: GNU GPL version 3 or later"
		" <http://gnu.org/licenses/gpl.html>.\n"
		"This is free software : you are free to change and redistribute it.\n"
		"\tThere is NO WARRANTY, to the extent permitted by law.\n"
		"\nWritten by Paul A. Chernick, Toby Speight and Edward Beroset\n";
	// flush the buffer to make sure the entire message is visible
	std::cout << std::flush;
	exit(EXIT_SUCCESS);
}

#ifdef _DEBUG
void ProgramOptions::singleLine(std::string flag, bool flagValue)
{
	std::cout << "Flag " << flag << ": Flag value " <<
		((flagValue) ? "True" : "False") << "\n";
}

void ProgramOptions::debugPrint()
{
	singleLine("blankLineCount", blankLineCount);
	singleLine("byteCount", byteCount);
	singleLine("charCount", charCount);
	singleLine("codeCount", codeCount);
	singleLine("commentCount", commentCount);
	singleLine("lineCount", lineCount);
	singleLine("maxLineWidth", maxLineWidth);
	singleLine("percentages", percentages);
	singleLine("whitespaceCount", whitespaceCount);
	singleLine("wordCount", wordCount);
	singleLine("recurseSubDirectories", recurseSubDirectories);
	singleLine("enableExecutionTime", enableExecutionTime);
}
#endif
