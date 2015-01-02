//-------------- Pawn Methods --------------

std::string connect_pawn(std::string msgs, void *com)
{
	int pid;
	std::string name, team;

	pid = atoi(getMessageElement(msgs, 1, false).c_str());
	team = getMessageElement(msgs, 2, false);
	name = getMessageElement(msgs, 3, false);

	Game *game = Game::getInstance();
	game->addUser(pid, team, name);

	std::string ret = std::string(CONNECT_PAWN_BACK_MSG);
	ret.append(team);
	ret.append(":");
	ret.append(name);

	return ret;
}

std::string replyToHello(std::string msgs, void *com)
{
	return std::string(HELLO_BACK_MSG);
}

std::string sendPath(std::string msgs, void *com)
{
	return std::string();
}

std::string sendMessage(std::string msgs, void *com)
{
	return std::string();
}

std::string doContinue(std::string msgs, void *com)
{
	return std::string();
}

std::string getCellState(std::string msgs, void *com)
{
	return std::string();
}

std::string getPath(std::string msgs, void *com)
{
	return std::string();
}

std::string getPosition(std::string msgs, void *com)
{
	return std::string();
}

std::string getGrade(std::string msgs, void *com)
{
	return std::string();
}

std::string getColor(std::string msgs, void *com)
{
	return std::string();
}


//-------------- Loader Methods --------------

std::string start_round(std::string msg, void *com)
{
	//stop loader
	((ComServer*)com)->pauseListening();

	//start pawns
	comPawns.startListening();

	return std::string(START_ROUND_BACK_MSG);
}

//TO DO: to verify if the one who sends start round and connect pawn is the loader 
std::string end_round(std::string msg, void *com)
{
	//stop listening for pawns
	comPawns.pauseListening();

	//start listening for loader
	((ComServer*)com)->startListening();

	return std::string(END_ROUND_BACK_MSG);
}