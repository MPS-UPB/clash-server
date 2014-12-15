#pragma once

//reads a command from a message and calls a function

#include <string>
#include <map>
#include <vector>
#include "util.h"

class ComInterpreter
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
	typedef std::string(*callbackFunction)(std::string, void*);

	std::map<std::string, callbackFunction, compCommand> commands;

public:
	ComInterpreter();

	//add functions to be called for a speciffic command
	void addListener(char *command, callbackFunction func);

	//remove functions, in order not to be called by a command
	void removeListener(char *command);

	//interpret message and call function associated with it; else return unknown command
	std::string interpret(char *message);

	~ComInterpreter();

private:
	char* getCommand(char *msg, char *ret, int dim_ret);
};
