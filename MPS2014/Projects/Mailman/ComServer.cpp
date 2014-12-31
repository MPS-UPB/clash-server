#include "ComServer.h"

//TO DO: use timeout
ComServer::ComServer(char *address, int nr_threads, int timeout)
{
	worker_threads = new std::vector<pthread_t>();
	worker_threads->resize(nr_threads);

	listening=false;
	this->address = _strdup(address);
	this->nr_threads = nr_threads;

	context = zmq_ctx_new();
}

void* ComServer::listeningLoop(void *instancev)
{
	ComServer *instance = (ComServer*)instancev;
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

void ComServer::startListening()
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


	// Launch pool of worker threads
	std::vector<pthread_t>::iterator it;
	for (it=worker_threads->begin(); it!=worker_threads->end(); it++)
	{
		int err = pthread_create(&(*it), &attr, this->listeningLoop, this);
		assert(err, "ERROR; cannot create enough threads in ComServer!");
	}

	//free attributes
	pthread_attr_destroy(&attr);

	// Connect work threads to client threads via a queue proxy
	zmq_proxy(clients, workers, NULL);
}

void ComServer::pauseListening()
{
	if (listening)
	{
		listening = false;
		zmq_close(clients);
		zmq_close(workers);
	}
}

ComServer::~ComServer()
{
	if (address)
		free(address);
	address = NULL;

	pauseListening();

	zmq_ctx_destroy(context);

	delete worker_threads;
}
