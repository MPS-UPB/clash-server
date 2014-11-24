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

public:
	Communication(char *echipa, char *nume)
	{
		printf ("%s from %s is connecting to the MPS server…\n", nume, echipa);
		context = zmq_ctx_new ();
		requester = zmq_socket (context, ZMQ_REQ);
		zmq_connect (requester, "tcp://localhost:5555");


		//talking
		std::string s;
		s.append(echipa);
		s.append(":");
		s.append(nume);

		char buffer[1024];

			printf ("%s Sending %s\n", s.c_str(),  s.c_str());
			zmq_send (requester, s.c_str(), s.length()+1, 0);

			zmq_recv (requester, buffer, 1024, 0);
			printf ("%s Received:%s\n", s.c_str(), buffer);
			mySleep(1000);
	}

	~Communication()
	{
	
		//TO DO: test for closing

			zmq_close (requester);
			zmq_ctx_destroy (context);

	}



	void sendMessage(std::string message)
	{

	}

	void sendMessage(char *msg, int len_msg, char *dst, int _len_dst)
	{
	}


};

#endif