#include "Communication.h"
#include "util\Addresses.h"
#include "util\Definitions.h"


Communication::Communication()
{
	com_requests = new ComClient(SERVER_ADDRESS_TO, TIMEOUT_SERVER_PAWN);
}

Communication::~Communication()
{
	delete com_requests;
}



void Communication::init() 
{
	//Override
}


void Communication::start()
{
	//Override
}


void Communication::stop()
{
	//Override
}


void Communication::wantToContinue()
{
	//Override
}


void Communication::doContinue()
{

}


void Communication::sendPath(std::string path)
{

}


int Communication::sendMessage(std::string message, std::string id)
{
	return 0;
}


void Communication::messageFailed(std::string pawn_id, int message_id)
{
	//Override
}


void Communication::receiveMessage(std::string sender_id, std::string message)
{
	//Override
}


std::string Communication::getCellState(int x, int y, int time)
{
	return std::string();
}


int Communication::getBoardSize()
{
	return 13;
}

int Communication::getThinkingTime()
{
	return 1000;
}

std::string Communication::getPath(std::string id, int t1, int t2)
{
	return std::string();
}


std::string Communication::getPosition(std::string id)
{
	return std::string();
}


std::string Communication::getGrade(std::string id)
{
	return std::string();
}


std::string Communication::getColor(std::string id)
{
	return std::string();
}

