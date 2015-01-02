#include <map>
#include "Pawn.h"

class Players
{
private:
	class compId
	{
	public:
		bool operator()(const std::string &x, const std::string &y) const
		{
			return x<y;
		}
	};

	std::map<std::string, Pawn, compId> *pawns;

public:
	Players();

	void addPawn(Pawn p);

	Pawn* getPawn(std::string id);

	~Players();
};