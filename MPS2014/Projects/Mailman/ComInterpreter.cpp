#include "ComInterpreter.h"

ComInterpreter::ComInterpreter()
{
	commands = new std::map<std::string, callbackFunction, compCommand>();
}

void ComInterpreter::addListener(std::string command, callbackFunction func)
{
	(*commands)[command] = func;
}

void ComInterpreter::removeListener(std::string command)
{
	commands->erase(command);
}

std::string ComInterpreter::interpret(std::string message)
{
	std::map<std::string, callbackFunction, compCommand>::iterator it;

	std::string cmd = getCommand(message);
	it = commands->find(cmd);

	if (it == commands->end())
	{
		return std::string("Unknown Command!\n");
	}
	else
	{
		return it->second(message, this);
	}
}

ComInterpreter::~ComInterpreter()
{
	delete commands;
}

//TO DO: FIXED: test if works ok
std::string ComInterpreter::getCommand(std::string msg)
{
	size_t end_cmd = msg.find(':');
	if (end_cmd == std::string::npos)
		return std::string(":");

	return msg.substr(0, end_cmd + 1);
}