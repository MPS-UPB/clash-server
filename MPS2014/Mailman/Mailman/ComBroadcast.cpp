#include "ComBroadcast.h"

ComBroadcast::ComBroadcast(char *address, int timeout)
{
	this->address = _strdup(address);

	context = zmq_ctx_new();

	//Socket to talk to clients
	listeners = zmq_socket(context, ZMQ_REQ);	//TO DO, protocol
	zmq_bind(listeners, address);
}

//TO DO: verify that buffer ends with 0
//TO DO: larger messages
void ComBroadcast::sendMessage(char *message)
{
	char buffer[1024];

	strcpy_s(buffer, 1024, message);

	zmq_send(listeners, buffer, strlen(buffer) + 1, 0);
}

ComBroadcast::~ComBroadcast()
{
	if (address)
		free(address);
	address = NULL;

	zmq_close(listeners);
	zmq_ctx_destroy(context);
}