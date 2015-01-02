#include "ComServer.h"
#include "util\Definitions.h"

//TO DO: use timeout
ComServer::ComServer(char *address, int nr_threads, int timeout)
{
	active_threads = 0;
	worker_threads = new std::vector<pthread_t>();
	worker_threads->resize(nr_threads);

	listening=false;
	this->address = _strdup(address);
	this->nr_threads = nr_threads;

	context = zmq_ctx_new();
}

//TO DO: nu stiu de ce vrea sa returneze un pointer functia pt pthread
void* ComServer::listeningLoop(void *instancev)
{
	ComServer *instance = (ComServer*)instancev;

	instance->active_threads++;

	void *client = zmq_socket(instance->context, ZMQ_REP);
	int rc = zmq_bind(client, "inproc://workers");
	assert(rc == 0, "Cannot bind to socket in ComServer thread!");

	while (instance->listening)
	{
		std::string query;

		int64_t more_messages;
		size_t more_size = sizeof more_messages;

		//receive query as multiple message fragments
		do
		{
			char buffer[MESSAGE_LENGTH_DEF];
			int recv_size = zmq_recv(client, buffer, MESSAGE_LENGTH_DEF, 0);
			assert(recv_size > 0, "Cannot receive query in ComServer thread!");
			query.append(buffer, recv_size);

			int err = zmq_getsockopt(client, ZMQ_RCVMORE, &more_messages, &more_size);
			assert(err == 0, "Cannot read socket information in ComServer thread!");
		} while (more_messages);

		//construct response
		std::string response = instance->interpret(query);

		int len = response.size();
		const char *msg_data = response.data();
		int iterator = 0;

		//send response fragments MESSAGE_LENGTH_DEF in size
		while (iterator < len)
		{
			//get a fragment
			int substr_len = min(len, MESSAGE_LENGTH_DEF);

			int more_messages = 0;
			if (len - iterator - substr_len > 0)
				more_messages = ZMQ_SNDMORE;

			//send the fragment
			int error = zmq_send(client, &(msg_data[iterator]), substr_len, more_messages);
			assert(error == substr_len, "Cannot send response in ComServer thread!");

			//iterate through the data
			iterator += substr_len;
		}
	}

	zmq_close(client);

	instance->active_threads--;

	pthread_exit(NULL);

	return NULL;
}

void ComServer::startListening()
{
	//Socket to talk to clients
	clients = zmq_socket(context, ZMQ_ROUTER);
	zmq_bind(clients, address);

	// Socket to talk to workers
	workers = zmq_socket(context, ZMQ_DEALER);
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

		while (active_threads > 0)
		{
			mySleep(100);
		}
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
