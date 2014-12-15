#pragma once

#include "ComInterpreter.h"
#include "zeromq/include/zmq.h"
#include "tinythread/include/tinythread.h"

//Used for sending messages to multiple Listeners
//Doesn't verify for receival -> it's the client's fault that it chrashed

class ComBroadcast
{
private:
	void *context;	//ComServer context
	char *address;

	void *listeners;	//socket for communicating to listeners

public:
	ComBroadcast(char *address, int timeout);

	void sendMessage(char *message);

	~ComBroadcast();
};