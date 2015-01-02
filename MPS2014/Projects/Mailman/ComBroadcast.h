#pragma once

#ifdef MAILMAN_EXPORTS
#define MAILMAN_API __declspec(dllexport)
#else
#define MAILMAN_API __declspec(dllimport)
#endif

#include "zeromq/include/zmq.h"

#pragma comment(lib, "util.lib")
#include "util/util.h"


//Used for sending messages to multiple Listeners
//Doesn't verify for receival -> it's the client's fault that it chrashed
//protocol: PUB from PUB/SUB

//TO DO: implement timeout; timeout -1=infinite
class MAILMAN_API ComBroadcast
{
private:
	void *context;	//ComServer context
	char *address;

	void *listeners;	//socket for communicating to listeners

public:
	ComBroadcast(char *address, int timeout);

	void sendMessage(std::string message);

	~ComBroadcast();
};