#include "ComListener.h"
#include "util\Definitions.h"

ComListener::ComListener(char *address)
{
	listening_thread = NULL;// new tthread::thread();
	active_threads = 0;

	listening = false;
	this->address = _strdup(address);

	context = zmq_ctx_new();
}

void* ComListener::listeningLoop(void *instancev)
{
	ComListener *instance = (ComListener*)instancev;

	instance->active_threads++;

	void *client = zmq_socket(instance->context, ZMQ_REP);
	int rc = zmq_bind(client, "inproc://workers");
	assert(rc == 0, "Cannot bind to socket in ComListener thread!");

	while (instance->listening)
	{
		std::string message;

		int64_t more_messages;
		size_t more_size = sizeof more_messages;

		//receive query as multiple message fragments
		do
		{
			char buffer[MESSAGE_LENGTH_DEF];
			int recv_size = zmq_recv(client, buffer, MESSAGE_LENGTH_DEF, 0);
			assert(recv_size > 0, "Cannot receive query in ComListener thread!");
			message.append(buffer, recv_size);

			int err = zmq_getsockopt(client, ZMQ_RCVMORE, &more_messages, &more_size);
			assert(err == 0, "Cannot read socket information in ComListener thread!");
		} while (more_messages);

		instance->interpret(message);
	}

	zmq_close(client);

	instance->active_threads--;

	pthread_exit(NULL);

	return NULL;
}

void ComListener::startListening()
{
	//Socket to listen to broadcaster
	broadcaster = zmq_socket(context, ZMQ_ROUTER);
	zmq_bind(broadcaster, address);

	// Socket to talk to workers
	workers = zmq_socket(context, ZMQ_DEALER);
	zmq_bind(workers, "inproc://workers");

	//Attributes for threads
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	//Start listening thread
	int err = pthread_create(listening_thread, &attr, this->listeningLoop, this);
	assert(err, "ERROR; cannot create enough threads in ComListener!");

	//free attributes
	pthread_attr_destroy(&attr);

	//Connect work threads to client threads via a queue proxy
	zmq_proxy(broadcaster, workers, NULL);
}

void ComListener::pauseListening()
{
	if (listening)
	{
		listening = false;
		zmq_close(broadcaster);
		zmq_close(workers);

		while (active_threads > 0)
		{
			mySleep(100);
		}

		//TO DO: nu stiu daca nu trebuie eliberat cumva threadul
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