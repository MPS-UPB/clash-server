#include "ComClient.h"

ComClient::ComClient(char *address, int timeout)
{
	this->address = _strdup(address);

	context = zmq_ctx_new();

	//Socket to talk to clients
	server = zmq_socket(context, ZMQ_REQ);
	zmq_bind(server, address);	
}

//TO DO: verify that buffer ends with 0
//TO DO: larger messages
std::string ComClient::sendMessage(char *message)
{
	char buffer[1024];

	strcpy_s(buffer, 1024, message);

	zmq_send(server, buffer, strlen(buffer)+1, 0);

	zmq_recv(server, buffer, 1024, 0);

	std::string ret = interpret(buffer);

	return ret;
}

ComClient::~ComClient()
{
	if (address)
		free(address);
	address = NULL;

	zmq_close(server);
	zmq_ctx_destroy(context);
}