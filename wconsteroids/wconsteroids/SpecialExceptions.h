#ifndef SPECIAL_EXCEPTIONS_H
#define SPECIAL_EXCEPTIONS_H

/*
 * Implements special exceptions for the --help and --version flags.
 */

#include <stdexcept>

class ShowHelpMessage: public std::runtime_error
{
public:
	ShowHelpMessage(const char* msg) : std::runtime_error(msg)
	{
	}
};

class showVersions : public std::runtime_error
{
public:
	showVersions(const char* msg) : std::runtime_error(msg)
	{

	}

};

#endif // SPECIAL_EXCEPTIONS_H