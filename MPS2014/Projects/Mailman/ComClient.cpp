#include "ComClient.h"
#include "util\Definitions.h"

ComClient::ComClient(char *address, int timeout)
{
	this->address = _strdup(address);

	context = zmq_ctx_new();

	//Socket to talk to clients
	server = zmq_socket(context, ZMQ_REQ);
	zmq_bind(server, address);	
}

std::string ComClient::sendMessage(std::string message)
{
	int len = message.size();
	const char *msg_data = message.data();
	int iterator=0;

	//send message fragments MESSAGE_LENGTH_DEF in size
	while (iterator < len)
	{
		//get a fragment
		int substr_len = min(len, MESSAGE_LENGTH_DEF);
		
		int more_messages = 0;
		if (len - iterator - substr_len > 0)
			more_messages = ZMQ_SNDMORE;

		//send the fragment
		int error = zmq_send(server, &(msg_data[iterator]), substr_len, more_messages);
		assert(error == substr_len, "Cannot send message in ComClient!");

		//iterate through the data
		iterator += substr_len;
	}

	std::string response;

	int64_t more_messages;
	size_t more_size = sizeof more_messages;
	do
	{
		char buffer[MESSAGE_LENGTH_DEF];
		int recv_size = zmq_recv(server, buffer, MESSAGE_LENGTH_DEF, 0);
		assert(recv_size>0, "Cannot receive response in ComClient!");
		response.append(buffer, recv_size);

		int err = zmq_getsockopt(server, ZMQ_RCVMORE, &more_messages, &more_size);
		assert(err == 0, "Cannot read socket information in ComClient!");
	} while (more_messages);
	

	std::string ret = interpret(response);

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