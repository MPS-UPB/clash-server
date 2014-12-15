#include "PawnExample.h"
#include <iostream>
using namespace std;

void PawnExample::init()
{
	time = 0;
	cout << "Round Started!\n";
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
	cout << "Message " << messageFailed << " could not be sent to " << pawn_id.c_str()<<"\n";
}

