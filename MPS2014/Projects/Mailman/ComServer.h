#pragma once

#ifdef MAILMAN_EXPORTS
#define MAILMAN_API __declspec(dllexport)
#else
#define MAILMAN_API __declspec(dllimport)
#endif

//protool: REP from REQ/REP
//Used for receiving messages from multiple Clients and replying with an answer

#define ZMQ_STATIC

#include <string>
#include <map>
#include <vector>
#include "ComInterpreter.h"
#include "zeromq/include/zmq.h"
#include "pthreads\include\pthread.h"

#pragma comment(lib, "util.lib")
#include "util/util.h"


//TO DO: implement timeout; timeout -1=infinite
class MAILMAN_API ComServer : public ComInterpreter
{
private:
	void *context;	//ComServer context
	char *address;
	char *internal_address;	//address for communication with workers

	void *clients;	//socket for listening for incomming clients
	void *workers;	//socket for sending work to worker threads

	int nr_threads;
	int active_threads;

	bool listening;

	std::vector<pthread_t> *worker_threads;

	
public:
	//TO DO: use internal address, maibe not working with inproc://workers
	ComServer(char *address, int nr_threads, int timeout);

	void startListening();

	void pauseListening();

	~ComServer();

private:

	static void* listeningLoop(void *instance);
};
