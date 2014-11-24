#ifndef COMMUNICATIONH
#define COMMUNICATIONH

#define ZMQ_STATIC
#include "zeromq/include/zmq.h"
#include <string>
#include <iostream>
#include "util.h"

class Communication
{
	void *context;
	void *responder;

public:
	Communication()
	{
		context = zmq_ctx_new ();
		responder = zmq_socket (context, ZMQ_REP);
		int rc = zmq_bind (responder, "tcp://*:5555");
		assert (rc == 0);

		std::cout<<"Waiting for incomming connections.\n";

		while (1) 
		{
			char buffer [1024];
			zmq_recv (responder, buffer, 1024, 0);
			printf ("SERVER Received: %s\n", buffer);
			mySleep (1);
			printf ("SERVER sending: %s\n", buffer);
			zmq_send (responder, buffer, strlen(buffer)+1, 0);
		}
	}



	~Communication()
	{

	}

	void sendMessage(std::string *message)
	{

	}

	void sendMessage(char *msg, int len_msg, char *dst, int _len_dst)
	{
	}


};

#endif