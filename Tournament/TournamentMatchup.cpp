#include "TournamentMatchup.h"

#define LOWER_SEED 0
#define HIGHER_SEED 1

TournamentMatchup::TournamentMatchup(Team &lower, Team &higher) : lowerSeed(lower), higherSeed(higher)
{
	currentGame = 1;
}


TournamentMatchup::~TournamentMatchup()
{
}

Team * TournamentMatchup::getWinner()
{
	if (gamesWon[LOWER_SEED] == REQUIRED_WINS)
		return &lowerSeed;
	if (gamesWon[HIGHER_SEED] == REQUIRED_WINS)
		return &higherSeed;
	else
		return NULL;
}

Team * TournamentMatchup::getLoser()
{
	if (gamesWon[LOWER_SEED] == REQUIRED_WINS)
		return &higherSeed;
	if (gamesWon[HIGHER_SEED] == REQUIRED_WINS)
		return &lowerSeed;
	return NULL;
}

bool TournamentMatchup::matchFinished(Team *winner)
{

	gamesWon[*winner == getHigherSeed() ? HIGHER_SEED : LOWER_SEED] += 1;

	currentGame++;

	return getWinner() != NULL;
}

Team & TournamentMatchup::getHigherSeed()
{
	return higherSeed;
}

Team & TournamentMatchup::getLowerSeed()
{
	return lowerSeed;
}
