#include "Pawn.h"
#include "util\CommonMessages.h"

Pawn::Pawn(std::string exe, std::string team, std::string name)
{
	executable = exe;
	this->team = team;
	this->name = name;

	id = std::string(team);
	id.append(":");
	id.append(name);

	pid = -1;
}

Pawn::Pawn()
{
	pid = -1;
}

void Pawn::start()
{
	if (pid != -1 || executable.length()<1)
		return;

	PROCESS_INFORMATION pi = runProcess(executable.c_str(), NULL);
	pid=pi.dwProcessId;
}

void Pawn::terminate()
{
	if (pid == -1)
		return;

	killProcess(pid);
	pid = -1;
}

void Pawn::restart()
{
	terminate();
	start();
}

int Pawn::getPID()
{
	return pid;
}

std::string Pawn::getId()
{
	return id;
}

void Pawn::connectToServer(ComClient *mailman_server)
{
	if (getId().length() < 1)
		return;

	printf("LOADER: Connecting %s...\n", getId());

	//construct message for server
	std::string message(CONNECT_PAWN_MSG);
	

	message.append(toString(getPID()));
	message.append(":");
	message.append(getId());

	//send message
	pawn_connected = false;
	std::string response = mailman_server->sendMessage(message);

	if (getMessageElement(response, 0).compare(CONNECT_PAWN_BACK_MSG) == 0)
		printf("LOADER: Connected!\n");
	else
		printf("LOADER: Error connecting pawn:%s\n", response);
}
