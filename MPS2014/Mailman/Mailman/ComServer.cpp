#include "ComServer.h"

//TO DO: use timeout
ComServer::ComServer(char *address, int nr_threads, int timeout)
{
	listening=false;
	this->address = _strdup(address);
	this->nr_threads = nr_threads;

	context = zmq_ctx_new();
}

void ComServer::listeningLoop()
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

void ComServer::startListening()
{
	//Socket to talk to clients
	void *clients = zmq_socket(context, ZMQ_ROUTER);
	zmq_bind(clients, address);

	// Socket to talk to workers
	void *workers = zmq_socket(context, ZMQ_DEALER);
	zmq_bind(workers, "inproc://workers");

	// Launch pool of worker threads
	for (int i = 0; i < nr_threads; i++)
	{
		tthread::thread worker(this->listeningLoop, NULL);
		worker.detach();
		worker_threads.push_back(worker);
	}

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
}
