#ifndef COMMUNICATIONH
#define COMMUNICATIONH

#include "zeromq/include/zmq.h"
#include <string>
#include<iostream>
#include "Mailman\ComClient.h"

#pragma comment (lib, "Mailman.lib")

class Communication
{
private:
	//communication chanel
	void *context_query;
	//socket for asking the server
	void *socket_query;
	//socket for receiving indications from server
	void *socket_command;

	ComClient *com_requests;

public:
	/**
	Constructor. Initializes communication.
	Probably won't have parameters in lter versions.
	*/
	Communication();
	//---- cere serverului sa ii dea init.
	//---- serverul asteapta de la loader sa ii dea OK-ul
	//---- loaderul porneste clientii si cand termina trimite OK la server
	//---- cand serverul primeste OK-ul de la loader, raspunde la clienti

	/**
	This function gets called when everything is ready.
	You should override this in order to know when the game starts.
	*/
	virtual void init();

	/**
	This function is called when you can start communicating with the server and pawns.
	You should override this in order to know when you can move the pawn.
	*/
	virtual void start();

	/**
	This function gets called when the server is no longer listening.
	You should override this in order to know when the time is up for a move.
	*/
	virtual void stop();

	/**
	This function is called at the end of a round (when a team wins).
	You should override this function if you want to keep the pawn active between games,
	for example, if you have a learning algorithm that learns with every round.
	If you want to continue without closing your executable, respond by calling doContinue().
	If you don't respond, your pawn will be terminated and the executable run in a different
	process. Restarting is helpful for crashing pawns or if you want to reset everything 
	between rounds.	If you do respond, at the beginning of the comming round the method 
	init() will be called again.
	*/
	virtual void wantToContinue();

	/**
	You use this function to answer inside wantToContinue() if you don't want to reset your 
	program when the following round starts.
	Do not call this function outside of wantToContinue() or more than once!
	*/
	void doContinue();

	/**
	Send the path you want to follow. Any precedent paths sent by you will be overwritten.
	The first letter is the movement you will be doing as soon as you can move.
	You will move one leter per tick.
	If you send an empty string, you will not move until you provide a nonempty path.
	The function is blocking. It stops here untill it receives a response.
	@param path a string containing 'U', 'D', 'L', and 'R'.
	*/
	void sendPath(std::string path);

	/**
	Send a message to a pawn.
	This function is non-blocking. If some recipients don't answer in a given ammount of 
	time you will be informed by the function messageFailed().
	@param message anything that can be put in a string and for which .lenght() works ok.
	@param id the id of the pawn. It has the form: team name:player name
	@return a message id that will be returned by the function messageSent() if the 
	message got through
	*/
	int sendMessage(std::string message, std::string id);

	/**
	This function gets called when the recipient does not answer in a given ammount of time,
	probably because it crashed. The message will not be automatically resent.
	You should override this function in order to know if a particular message didn't get 
	through.
	@param pawn_id the id of the pawn that did not answer to the message
	@param message_id the id returned by the function sendMessage()
	*/
	virtual void messageFailed(std::string pawn_id, int message_id);
	
	/**
	 * The server calls this function when someone sends a message tp you.
	 * Ypu have to override ot in order tomreceove messages.
	 */
	 
	virtual void receiveMessage(std::string sender_id, std::string message);

	/**
	Returns the id of a pawn occupying the respective cell if it will not be empty at the
	given time, or an empty string ("") otherwise.
	The function is blocking. It stops here untill it receives a response.
	@param x column, starting from 0 from left to right
	@param y row, starting from 0 from top to bottom
	@param time, how many moves in the future or the past. 0=current time, negative=past, 
	positive=future
	@return a string in the form of an id: "team:name" of the occupier, or ""
	*/
	std::string getCellState(int x, int y, int time);
	
	/**
	 * Returns the size of the board edge. The board is square.
	 * @return size of an egde
	 */
	int getBoardSize();
	
	/**
	 * Returns how many miliseconds you have between start() and stop()
	 * @return number of miliseconds between start and stop
	 */
	int getThinkingTime();

	/**
	Returns the path of a pawn from the starting time t1 till t2. If the player didn't offer 
	paths untill t2, the resulting string will be shorter.
	The function is blocking. It stops here untill it receives a response.
	@param id the id of the pawn. It has the form: team name:player name
	@param t1 the start time of the path. Negative for history recordings, 0 from current.
	@param t2 the end time for the path. Whtill when are you interested in the pawn's moves.
	@return a string containing 'U', 'D', 'L', and 'R' from t1 till at most t2. 
	*/
	std::string getPath(std::string id, int t1, int t2);

	/**
	Returns the position of a pawn on the board.
	The function is blocking. It stops here untill it receives a response.
	@param id the id of the pawn. It has the form: team name:player name
	@return a string of the form "(x,y)", where x and y are two integers. They are -1 if 
	the player is not on the board. x is starting from 0 from left to right; y is starting 
	from 0 from top to bottom.
	*/
	std::string getPosition(std::string id);

	/**
	Returns the points accumulated untill now by a pawn.
	The function is blocking. It stops here untill it receives a response.
	@param id the id of the pawn. It has the form: team name:player name
	@return a string of the form "(i,j)", where i and j are integers. i is the individual 
	grade and j the team grade.
	*/
	std::string getGrade(std::string id);

	/**
	Returns the color (apartenence to a team) of a pawn.
	The function is blocking. It stops here untill it receives a response.
	@param id the id of the pawn. It has the form: team name:player name
	@return a string of the form "(i,j)", where i and j are integers. i is the luminosity
	(the original team he belongs to) and j is the color (the team for which it is playing 
	this round)
	*/
	std::string getColor(std::string id);

	/**
	Destructor. Closes communication channels.
	*/
	~Communication();
};


#endif
