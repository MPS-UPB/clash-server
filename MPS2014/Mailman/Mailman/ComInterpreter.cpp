#include "ComInterpreter.h"

ComInterpreter::ComInterpreter()
{
}

void ComInterpreter::addListener(char *command, callbackFunction func)
{
	commands[std::string(command)] = func;
}

void ComInterpreter::removeListener(char *command)
{
	commands.erase(command);
}

std::string  ComInterpreter::interpret(char *message)
{
	char buffer[1024];

	std::map<std::string, callbackFunction, compCommand>::iterator it;

	char *cmd = getCommand(message, buffer, 1024);
	it = commands.find(cmd);

	if (it == commands.end())
	{
		strcpy_s(buffer, 1024, "Unknown Command!\n");
		return std::string(buffer);
	}
	else
	{
		return it->second(std::string(message), this);
	}
}

ComInterpreter::~ComInterpreter()
{
}

char* ComInterpreter::getCommand(char *msg, char *ret, int dim_ret)
{
	//printf("Entire command: %s\n", msg);
	int nr = strchr(msg, ':') - msg + 1;
	strncpy_s(ret, dim_ret, msg, nr);
	//printf("Computed command: %s\n", ret);
	return ret;
}