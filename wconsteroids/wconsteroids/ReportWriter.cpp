#include <iostream>
#include <string>
#include <vector>
#include "FileStatistics.h"
#include "ProgramOptions.h"
#include "ReportWriter.h"

void ReportWriter::printResult(FileStatistics& resultsForOutput) noexcept
{
	std::cout << getResultText(resultsForOutput) << "\n";
}

/*
 * Maintain the order between this function and getColumneHeadingsText().  
 */
std::string ReportWriter::getResultText(FileStatistics& resultsForOutput) noexcept
{
	std::string outString;

	if (options.lineCount)
	{
		outString += std::to_string(resultsForOutput.getTotalLines()) + "\t";
	}

	if (options.wordCount)
	{
		outString += std::to_string(resultsForOutput.getWords()) + "\t";
	}

	if (options.byteCount)
	{
		outString += std::to_string(resultsForOutput.getCharacters()) + "\t";
	}

	if (options.charCount)
	{
		outString += std::to_string(resultsForOutput.getCharacters()) + "\t\t";
	}

	if (options.maxLineWidth)
	{
		outString += std::to_string(resultsForOutput.getWidestLine()) + "\t";
	}

	// End of backwards compatability with wc utility.

	if (options.codeCount)
	{
		outString += std::to_string(resultsForOutput.getCodeLines()) + "\t";
	}

	if (options.commentCount)
	{
		outString += std::to_string(resultsForOutput.getCommentLines()) + "\t";
	}

	if (options.percentages)
	{
		outString +=
			std::to_string(resultsForOutput.getPerecentageOfCode()) + "\t";
	}

	if (options.whitespaceCount)
	{
		outString += std::to_string(resultsForOutput.getWhitespace()) + "\t";
	}
	if (options.blankLineCount)
	{
		outString += std::to_string(resultsForOutput.getBlankLines()) + "\t";
	}

	std::string fileName = correctFileSpec(resultsForOutput.getFileName());
	outString += "\t" + fileName + "\n";

	return outString;
}

/*
 * Maintain the order between this function and getResultText().
 */
std::vector<std::string> ReportWriter::getColumneHeadingsText() noexcept
{
	std::string firstLine;
	std::string secondline;

	if (options.lineCount)
	{
		firstLine += "Lines\t";
		secondline += "of Text\t";
	}

	if (options.wordCount)
	{
		firstLine += "Words\t";
		secondline += "\t";
	}

	if (options.byteCount)
	{
		firstLine += "Bytes\t";
		secondline += "\t";
	}

	if (options.charCount)
	{
		firstLine += "Characters\t";
		secondline += "\t\t";
	}

	if (options.maxLineWidth)
	{
		firstLine += "Length of\t";
		secondline += "Longest Line\t";
	}

	// End of backwards compatability with wc utility.

	if (options.codeCount)
	{
		firstLine += "Lines\t";
		secondline += "of Code\t";
	}

	if (options.commentCount)
	{
		firstLine += "Lines of\t";
		secondline += "Comments\t";
	}

	if (options.percentages)
	{
		firstLine += "Percentage of\t";
		secondline += "Lines of Code\t";
	}

	if (options.whitespaceCount)
	{
		firstLine += "Whitespace\t";
		secondline += "Characters\t";
	}
	if (options.blankLineCount)
	{
		firstLine += "Blank\t";
		secondline += "Lines\t";
	}


	std::vector<std::string> headerRows = { firstLine, secondline };
	return headerRows;
}

std::string ReportWriter::getColumnHeadingAsOneString() noexcept
{
	std::string twolines;
	std::vector<std::string> headingLines = getColumneHeadingsText();
	FileStatistics allFiles;

	std::string twoLines;
	for (auto headingLine : headingLines)
	{
		twoLines += headingLine + "\n";
	}

	return twoLines;
}

void ReportWriter::printColumnHeadings() noexcept
{
	std::vector<std::string> headerRows = getColumneHeadingsText();
	for (auto line : headerRows)
	{
		std::cout << line << "\n";
	}
}

std::string ReportWriter::correctFileSpec(std::string fileSpec) noexcept
{
	if (options.recurseSubDirectories)
	{
		return fileSpec;
	}
	else
	{
		auto newStart = fileSpec.find_last_of('/');
		if (!newStart)
		{
			newStart = fileSpec.find_last_of('\\');
			if (!newStart)
			{
				return fileSpec;
			}
		}
		std::string fileName = fileSpec.substr(newStart + 1);

		
		return fileName;
	}
}
