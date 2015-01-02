#pragma once

#ifdef MAILMAN_EXPORTS
#define MAILMAN_API __declspec(dllexport)
#else
#define MAILMAN_API __declspec(dllimport)
#endif

//reads a command from a message and calls a function

#include <string>
#include <map>
#include <vector>

#pragma comment(lib, "util.lib")
#include "util/util.h"


class MAILMAN_API ComInterpreter
{
private:
	class compCommand
	{
	public:
		bool operator()(const std::string &x, const std::string &y) const
		{
			return x<y;
		}
	};

	//function returning a reply to the message and receiving a message and the object calling the function
	//second parameter=pointer to this instance
	typedef std::string(*callbackFunction)(std::string, void*);

	std::map<std::string, callbackFunction, compCommand> *commands;

public:
	ComInterpreter();

	//add functions to be called for a speciffic command
	void addListener(std::string command, callbackFunction func);

	//remove functions, in order not to be called by a command
	void removeListener(std::string command);

	//interpret message and call function associated with it; else return unknown command
	std::string interpret(std::string message);

	~ComInterpreter();

private:
	//returns the command from a message (everything before the character ':', including ':')
	std::string getCommand(std::string);
};
