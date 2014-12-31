//-------------- Pawn Methods --------------

std::string connect_pawn(std::string msgs, void *com)
{
	int pid;
	std::string name, team;

	char *msg = _strdup(msgs.c_str());
	int len = strlen(msg);
	char *context = NULL;

	char *split = strtok_s(msg, ":", &context);

	pid = atoi(strtok_s(NULL, ":", &context));
	team = std::string(strtok_s(NULL, ":", &context));
	name = std::string(strtok_s(NULL, ":", &context));

	Game *game = Game::getInstance();
	game->addUser(pid, team.c_str(), name.c_str());

	std::string ret = std::string("added user: ");
	ret.append(team);
	ret.append(":");
	ret.append(name);

	free(msg);

	return ret;
}

std::string replyToHello(std::string msgs, void *com)
{
	return std::string("Hello back from SERVER!");
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

	return std::string("Round Started!");
}

//TO DO: to verify if the one who sends start round and connect pawn is the loader 
std::string end_round(std::string msg, void *com)
{
	//stop listening for pawns
	comPawns.pauseListening();

	//start listening for loader
	((ComServer*)com)->startListening();

	return std::string("Round Ended!");
}