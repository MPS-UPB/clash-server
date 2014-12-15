#pragma once

#include "ComInterpreter.h"
#include "zeromq/include/zmq.h"
#include "tinythread/include/tinythread.h"

//Used for receiving messages from a single Broadcast
//Spawns a new thread that listens in the background

class ComListener : public ComInterpreter
{
private:
	void *context;	//ComListener context
	char *address;

	void *broadcaster;	//socket for listening for broadcasts
	void *workers;	//socket for sending work to worker threads

	bool listening;

	tthread::thread listening_thread;

public:
	ComListener(char *address);

	void startListening();

	void pauseListening();

	~ComListener();

private:
	void listeningLoop();
};