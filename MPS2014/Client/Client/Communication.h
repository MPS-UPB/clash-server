#ifndef COMMUNICATIONH
#define COMMUNICATIONH

#include "zeromq/include/zmq.h"
#include <string>
#include<iostream>
#include "util.h"

class Communication
{
	void *context;
	void *requester;
	void *requester2;

public:
	Communication()
	{
		printf ("Connecting to hello world server…\n");
		context = zmq_ctx_new ();
		requester = zmq_socket (context, ZMQ_REQ);
		zmq_connect (requester, "tcp://localhost:5555");

		requester2 = zmq_socket (context, ZMQ_REQ);
		zmq_connect (requester2, "tcp://localhost:5555");

		int request_nbr;
		for (request_nbr = 0; request_nbr != 10; request_nbr++) 
		{
			char buffer [10];
			char buffer2 [10];
			printf ("1 Sending %s\n", "Hello1");
			zmq_send (requester, "Hello1", 7, 0);
			printf ("2 Sending %s\n", "Hello2");
			zmq_send (requester2, "Hello2", 7, 0);

			zmq_recv (requester, buffer, 10, 0);
			printf ("1 Received:%s\n", buffer);
			zmq_recv (requester2, buffer2, 10, 0);
			printf ("2 Received:%s\n", buffer2);
			mySleep(1000);
		}
	}

	~Communication()
	{
		int err=EINTR;
		//TO DO: maybe better options to deal with non closing context
		do
		{
			zmq_close (requester);
			zmq_ctx_destroy (context);
		}while(err==EINTR);
	}



	void sendMessage(std::string message)
	{

	}

	void sendMessage(char *msg, int len_msg, char *dst, int _len_dst)
	{
	}


};

#endif