#ifndef HELPME_H
#define HELPME_H
#include <exception>
class HelpMe: public std::runtime_error
{
public:
	HelpMe(const char* msg) : std::runtime_error(msg)
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

#endif // HELPME_H