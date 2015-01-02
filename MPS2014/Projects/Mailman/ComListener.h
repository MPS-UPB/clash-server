#pragma once

#ifdef MAILMAN_EXPORTS
#define MAILMAN_API __declspec(dllexport)
#else
#define MAILMAN_API __declspec(dllimport)
#endif

#include "ComInterpreter.h"
#include "zeromq/include/zmq.h"
#include "pthreads\include\pthread.h"

#pragma comment(lib, "util.lib")
#include "util/util.h"


//Used for receiving messages from a single Broadcast
//Spawns a new thread that listens in the background
//protocol: SUB from PUB/SUB

class MAILMAN_API ComListener : public ComInterpreter
{
private:
	void *context;	//ComListener context
	char *address;

	void *broadcaster;	//socket for listening for broadcasts
	void *workers;	//socket for sending work to worker threads

	bool listening;
	int active_threads;

	pthread_t *listening_thread;

public:
	ComListener(char *address);

	void startListening();

	void pauseListening();

	~ComListener();

private:
	static void* listeningLoop(void *instance);
};