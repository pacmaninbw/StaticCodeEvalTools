#include <iostream>
#include <string>
#include <vector>
#include "ReportWriter.h"

void ReportWriter::printResult(FileStatistics& resultsForOutput)
{
	std::cout << getResultText(resultsForOutput) << "\n";
}

/*
 * Maintain the order between this function and getColumneHeadingsText().  
 */
std::string ReportWriter::getResultText(FileStatistics& resultsForOutput)
{
	std::string outString;

	if (executionCtrl->lineCount)
	{
		outString += std::to_string(resultsForOutput.getTotalLines()) + "\t";
	}

	if (executionCtrl->wordCount)
	{
		outString += std::to_string(resultsForOutput.getWords()) + "\t";
	}

	if (executionCtrl->byteCount)
	{
		outString += std::to_string(resultsForOutput.getCharacters()) + "\t";
	}

	if (executionCtrl->charCount)
	{
		outString += std::to_string(resultsForOutput.getCharacters()) + "\t";
	}

	if (executionCtrl->maxLineWidth)
	{
		outString += std::to_string(resultsForOutput.getWidestLine()) + "\t";
	}

	// End of backwards compatability with wc utility.

	if (executionCtrl->codeCount)
	{
		outString += std::to_string(resultsForOutput.getCodeLines()) + "\t";
	}

	if (executionCtrl->commentCount)
	{
		outString += std::to_string(resultsForOutput.getCommentLines()) + "\t";
	}

	if (executionCtrl->percentages)
	{
		outString += std::to_string(resultsForOutput.getPerecentageOfCode()) + "\t";
	}

	if (executionCtrl->whitespaceCount)
	{
		outString += std::to_string(resultsForOutput.getWhitespace()) + "\t";
	}
	if (executionCtrl->blankLineCount)
	{
		outString += std::to_string(resultsForOutput.getBlankLines()) + "\t";
	}

	outString += "\t\t" + resultsForOutput.getFileName();

	return outString;
}

/*
 * Maintain the order between this function and getResultText().
 */
std::vector<std::string> ReportWriter::getColumneHeadingsText()
{
	std::string firstLine;
	std::string secondline;

	if (executionCtrl->lineCount)
	{
		firstLine += "Lines\t";
		secondline += "of Text\t";
	}

	if (executionCtrl->wordCount)
	{
		firstLine += "Words\t";
		secondline += "\t";
	}

	if (executionCtrl->byteCount)
	{
		firstLine += "Bytes\t";
		secondline += "\t";
	}

	if (executionCtrl->charCount)
	{
		firstLine += "Characters\t";
		secondline += "\t\t";
	}

	if (executionCtrl->maxLineWidth)
	{
		firstLine += "Length of\t";
		secondline += "Longest Line\t";
	}

	// End of backwards compatability with wc utility.

	if (executionCtrl->codeCount)
	{
		firstLine += "Lines\t";
		secondline += "of Code\t";
	}

	if (executionCtrl->commentCount)
	{
		firstLine += "Lines of\t";
		secondline += "Comments\t";
	}

	if (executionCtrl->percentages)
	{
		firstLine += "Percentage of\t";
		secondline += "Lines of Code\t";
	}

	if (executionCtrl->whitespaceCount)
	{
		firstLine += "Whitespace\t";
		secondline += "Characters\t";
	}
	if (executionCtrl->blankLineCount)
	{
		firstLine += "Blank\t";
		secondline += "Lines\t";
	}


	std::vector<std::string> headerRows = { firstLine, secondline };
	return headerRows;
}

void ReportWriter::printColumnHedings()
{
	std::vector<std::string> headerRows = getColumneHeadingsText();
	for (auto line : headerRows)
	{
		std::cout << line << "\n";
	}
}
