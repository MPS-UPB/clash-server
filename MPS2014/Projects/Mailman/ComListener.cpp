#include "ComListener.h"

ComListener::ComListener(char *address)
{
	listening_thread = NULL;// new tthread::thread();

	listening = false;
	this->address = _strdup(address);

	context = zmq_ctx_new();
}

//BUG: this is copy paste from server. Maybe there is a way to merge them in a single implementation
void* ComListener::listeningLoop(void *instancev)
{
	ComListener *instance = (ComListener*)instancev;
	void *receiver = zmq_socket(instance->context, ZMQ_REP);
	int rc = zmq_bind(receiver, "inproc://workers");
	assert(rc == 0);

	char recmsg[1024];
	char buffer[1024];

	while (instance->listening)
	{
		zmq_recv(receiver, recmsg, 1024, 0);
		printf("SERVER: Received: %s\n", recmsg);

		std::string ret = instance->interpret(recmsg);
		strcpy_s(buffer, 1024, ret.c_str());

		printf("SERVER: Sending: %s\n", buffer);
		zmq_send(receiver, buffer, strlen(buffer) + 1, 0);
	}

	zmq_close(receiver);

	pthread_exit(NULL);

	return NULL;
}

void ComListener::startListening()
{
	//Socket to talk to clients
	void *clients = zmq_socket(context, ZMQ_ROUTER);
	zmq_bind(clients, address);

	// Socket to talk to workers
	void *workers = zmq_socket(context, ZMQ_DEALER);
	zmq_bind(workers, "inproc://workers");

	//Attributes for threads
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	int err = pthread_create(listening_thread, &attr, this->listeningLoop, this);
	assert(err, "ERROR; cannot create enough threads in ComListener!");

	//free attributes
	pthread_attr_destroy(&attr);

	// Connect work threads to client threads via a queue proxy
	zmq_proxy(clients, workers, NULL);
}

void ComListener::pauseListening()
{
	if (listening)
	{
		listening = false;
		zmq_close(broadcaster);
		zmq_close(workers);

		listening_thread = NULL;
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