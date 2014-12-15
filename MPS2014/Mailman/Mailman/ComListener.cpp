#include "ComListener.h"

ComListener::ComListener(char *address)
{
	listening = false;
	this->address = _strdup(address);

	context = zmq_ctx_new();
}

//BUG: this is copy paste from server. Maybe there is a way to merge them in a single implementation
void ComListener::listeningLoop()
{
	void *receiver = zmq_socket(context, ZMQ_REP);
	int rc = zmq_bind(receiver, "inproc://workers");
	assert(rc == 0);

	char recmsg[1024];
	char buffer[1024];

	while (listening)
	{
		zmq_recv(receiver, recmsg, 1024, 0);
		printf("SERVER: Received: %s\n", recmsg);

		std::string ret = interpret(recmsg);
		strcpy_s(buffer, 1024, ret.c_str);

		printf("SERVER: Sending: %s\n", buffer);
		zmq_send(receiver, buffer, strlen(buffer) + 1, 0);
	}

	zmq_close(receiver);
}

void ComListener::startListening()
{
	//Socket to talk to clients
	void *broadcaster = zmq_socket(context, ZMQ_ROUTER);
	zmq_bind(broadcaster, address);

	// Socket to talk to workers
	void *workers = zmq_socket(context, ZMQ_DEALER);
	zmq_bind(workers, "inproc://workers");

	// Launch listening thread
	listening_thread = tthread::thread(this->listeningLoop, NULL);
	listening_thread.detach();

	// Connect work threads to client threads via a queue proxy
	zmq_proxy(broadcaster, workers, NULL);
}

void ComListener::pauseListening()
{
	if (listening)
	{
		listening = false;
		zmq_close(broadcaster);
		zmq_close(workers);
	}
}

ComListener::~ComListener()
{
	if (address)
		free(address);
	address = NULL;

	pauseListening();

	zmq_ctx_destroy(context);
}