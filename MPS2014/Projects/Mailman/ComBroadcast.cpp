#include "ComBroadcast.h"
#include "util\Definitions.h"

ComBroadcast::ComBroadcast(char *address, int timeout)
{
	this->address = _strdup(address);

	context = zmq_ctx_new();

	//Socket to talk to clients
	listeners = zmq_socket(context, ZMQ_PUB);
	zmq_bind(listeners, address);
}

void ComBroadcast::sendMessage(std::string message)
{
	int len = message.size();
	const char *msg_data = message.data();
	int iterator = 0;

	//send message fragments MESSAGE_LENGTH_DEF in size
	while (iterator < len)
	{
		//get a fragment
		int substr_len = min(len, MESSAGE_LENGTH_DEF);

		int more_messages = 0;
		if (len - iterator - substr_len > 0)
			more_messages = ZMQ_SNDMORE;

		//send the fragment
		int error = zmq_send(listeners, &(msg_data[iterator]), substr_len, more_messages);
		assert(error == substr_len, "Cannot send message in ComBroadcast!");

		//iterate through the data
		iterator += substr_len;
	}
}

ComBroadcast::~ComBroadcast()
{
	if (address)
		free(address);
	address = NULL;

	zmq_close(listeners);
	zmq_ctx_destroy(context);
}