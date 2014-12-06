#include "Communication.h"

Communication::Communication()
{
	listening=false;
	context = zmq_ctx_new ();
	responder = zmq_socket (context, ZMQ_REP);
	int rc = zmq_bind (responder, "tcp://*:5555");
	assert (rc == 0);
}

void Communication::addListener(char *command, callbackFunction func)
{
	commands[std::string(command)]=func;
}

void Communication::removeListener(char *command)
{
	commands.erase(command);
}

void Communication::listeningLoop()
{
	char recmsg[1024];
	char buffer[1024];
	while (1) 
	{
		zmq_recv (responder, recmsg, 1024, 0);
		printf("SERVER: Received: %s\n", recmsg);

		std::map<std::string, callbackFunction, compCommand>::iterator it;

		it=commands.find(getCommand(recmsg, buffer, 1024));

		if(it==commands.end())
		{
			strcpy_s(buffer, "Unknown Command!");
			zmq_send(responder, buffer, strlen(buffer)+1, 0);
		}
		else
		{
			std::string sbuffer=it->second(std::string(recmsg));
			zmq_send(responder, sbuffer.c_str(), sbuffer.length()+1, 0);
		}	
	}
}

void Communication::startListening()
{
	char recmsg[1024];
	char buffer[1024];
	while (1)
	{
		zmq_recv(responder, recmsg, 1024, 0);
		printf("SERVER: Received: %s\n", recmsg);

		if (strcmp(recmsg, "Hello from Loader!") == 0)
		{
			strcpy_s(buffer, "Unknown Command!");
			zmq_send(responder, buffer, strlen(buffer) + 1, 0);
		}
		else
		{
			strcpy_s(buffer, "Hello back from Server!");
			zmq_send(responder, buffer, strlen(buffer) + 1, 0);
		}
	}
}

Communication::~Communication()
{

}

char* Communication::getCommand(char *msg, char *ret, int dim_ret)
{
	int nr=strchr(msg, ':')-msg+1;
	strncpy_s(ret, dim_ret, msg, nr);
	return ret;
}