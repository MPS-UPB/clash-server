#include "PawnExample.h"
#include <iostream>
using namespace std;

void PawnExample::init()
{
	time = 0;
	cout << "Round Started!\n";

	std::string pos = getPosition("team 1:Client.exe");
	cout << "I am in: " << pos.c_str();
}

void PawnExample::start()
{
	cout << "Make your move!\n";
	
	sendPath("UDLRRRR");
}

void PawnExample::stop()
{
	time++;
	cout << "Time: "<<time<<"\n";
}

void PawnExample::wantToContinue()
{
	cout << "Round finished! Server is asking if I want to restart\n";

	//don't want to close
	doContinue();
}

void PawnExample::messageFailed(std::string pawn_id, int message_id)
{
	cout << "Message " << message_id << " could not be sent to " << pawn_id.c_str() << "\n";
}

