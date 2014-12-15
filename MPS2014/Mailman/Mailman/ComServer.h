#pragma once

//protool: REP from REQ/REP
//Used for receiving messages from multiple Clients and replying with an answer

#define ZMQ_STATIC

#include <string>
#include <map>
#include <vector>
#include "util.h"
#include "ComInterpreter.h"
#include "zeromq/include/zmq.h"
#include "tinythread/include/tinythread.h"

class ComServer : public ComInterpreter
{
private:
	void *context;	//ComServer context
	char *address;
	char *internal_address;	//address for communication with workers

	void *clients;	//socket for listening for incomming clients
	void *workers;	//socket for sending work to worker threads

	int nr_threads;

	bool listening;

	std::vector<tthread::thread> worker_threads;

	
public:
	//TO DO: use internal address
	ComServer(char *address, int nr_threads, int timeout);

	void startListening();

	void pauseListening();

	~ComServer();

private:

	void listeningLoop();
};
