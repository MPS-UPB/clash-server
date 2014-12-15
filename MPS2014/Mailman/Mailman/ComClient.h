#pragma once

#include "ComInterpreter.h"
#include "zeromq/include/zmq.h"

//Protocol: REQ from REQ/REP
//Used for sending messages to a single server and receiving answers from it, blocking

class ComClient : public ComInterpreter
{
private:
	void *context;	//ComServer context
	char *address;

	void *server;	//SeverSocker

public:
	ComClient(char *address, int timeout);

	std::string sendMessage(char *message);

	~ComClient();
};