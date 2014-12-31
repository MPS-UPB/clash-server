#include "Communication.h"

class PawnExample : public Communication
{
private:
	int time;
public:

	void init();


	void start();


	void stop();


	void wantToContinue();

	//implemented by server
	//void doContinue();

	//implemented by server
	//void sendPath(std::string path);

	//implemented by server
	//int sendMessage(std::string message, std::string id);


	void messageFailed(std::string pawn_id, int message_id);

	//implemented by server
	//std::string getCellState(int x, int y, int time);

	//implemented by server
	//std::string getPath(std::string id, int t1, int t2);

	//implemented by server
	//std::string getPosition(std::string id);

	//implemented by server
	//std::string getGrade(std::string id);

	//implemented by server
	//std::string getColor(std::string id);
};