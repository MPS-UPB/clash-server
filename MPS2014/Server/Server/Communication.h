#pragma once

#define ZMQ_STATIC
#include "zeromq/include/zmq.h"
#include "tinythread/include/tinythread.h"
#include <string>
#include <iostream>
#include <map>
#include <string>
#include <string.h>
#include "util.h"
#include "Pawn.h"


class Communication
{
private:
	void *context;
	void *responder;

	bool listening;

	class compCommand 
	{
	public:
		bool operator()(const std::string &x, const std::string &y) const
		{ 
			//unsigned int lx=x.length();
			//if(lx<y.length())
				//return false;

			// returns x>y
			return x<y;//x.compare(0, lx, y.c_str(), lx)<0;
		} 
	};

	typedef std::string (*callbackFunction)(std::string);

	std::map<std::string, callbackFunction, compCommand> commands;

	
public:
	Communication();

	void addListener(char *command, callbackFunction func);

	void removeListener(char *command);

	void listeningLoop();

	void startListening();

	~Communication();

private:
	char* getCommand(char *msg, char *ret, int dim_ret);


};
