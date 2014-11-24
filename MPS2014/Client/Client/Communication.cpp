#include "Communication.h"

Communication::Communication(char *echipa, char *nume)
{
	//Connect to the server
	printf ("%s from %s is connecting to the MPS server…\n", nume, echipa);
	context_query = zmq_ctx_new ();
	socket_query = zmq_socket (context_query, ZMQ_REQ);
	zmq_connect (socket_query, "tcp://localhost:5555");

	//Create ID message
	std::string s;
	s.append(echipa);
	s.append(":");
	s.append(nume);

	//Send ID message and receive OK
	char buffer[1024];

	printf ("%s Sending %s\n", s.c_str(),  s.c_str());
	zmq_send (socket_query, s.c_str(), s.length()+1, 0);

	zmq_recv (socket_query, buffer, 1024, 0);
	printf ("%s Received:%s\n", s.c_str(), buffer);
	mySleep(1000);
}

Communication::~Communication()
{
	//TO DO: test for closing
	zmq_close (socket_query);
	zmq_ctx_destroy (context_query);
}