#pragma once

#ifdef MAILMAN_EXPORTS
#define MAILMAN_API __declspec(dllexport)
#else
#define MAILMAN_API __declspec(dllimport)
#endif

#include "ComInterpreter.h"
#include "zeromq/include/zmq.h"

#pragma comment(lib, "util.lib")
#include "util/util.h"

//Protocol: REQ from REQ/REP
//Used for sending messages to a single server and receiving answers from it, blocking

//TO DO: implement timeout; timeout-1= infinite
class MAILMAN_API ComClient : public ComInterpreter
{
private:
	void *context;	//ComServer context
	char *address;

	void *server;	//SeverSocker

public:
	ComClient(char *address, int timeout);

	std::string sendMessage(std::string message);

	~ComClient();
};