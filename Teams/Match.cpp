#include "Match.h"

//#include "Team.h"
#include "..\Tournament\Tournament.h"

static const int HOME = 0, AWAY = 1;
static const int NOT_STARTED = 0, MAX_ROUNDS = 1, FINISHED = MAX_ROUNDS + 1;

Match::Match(Team &h, Team &a, DateTime d) : home(h), away(a) {
	scores[0] = 0;
	scores[1] = 0;
	date = d;
}

Match::Match(Team & h, Team & a, DateTime d, Tournament * tourn) : home(h), away(a)
{
	scores[0] = 0;
	scores[1] = 0;
	date = d;
	playoffs = tourn;
}

Match::~Match()
{
}

Team * Match::getWinner()
{
	if (scores[HOME] > scores[AWAY])
		return &home;
	if (scores[HOME] < scores[AWAY])
		return &away;
	return NULL;
}

Team * Match::getLoser()
{
	if (scores[HOME] < scores[AWAY])
		return &home;
	if (scores[HOME] > scores[AWAY])
		return &away;
	return NULL;
}

Team & Match::getHomeTeam()
{
	return home;
}

Team & Match::getAwayTeam()
{
	return away;
}

void Match::setScore(int score, bool home)
{
	scores[home ? HOME : AWAY] = score;
}

int Match::getScore(bool home)
{
	return scores[home ? HOME : AWAY];
}

// TODO
bool Match::start()
{
	// Match already started
	if (round != NOT_STARTED) {
		return false;
	}
	round += 1;
	return true;
}

// TODO
bool Match::finish()
{

	if (round == NOT_STARTED || round == FINISHED)
		return false;

	Team *winner = getWinner();

	if (winner == NULL)
		return false;

	round = FINISHED;

	winner->alterRecord(true);
	getLoser()->alterRecord(false);

	if (playoffs != NULL) {
		playoffs->teamWonGame(winner);
	}

	return true;

}

bool Match::isFinished()
{
	return round == FINISHED;
}

DateTime Match::getDate()
{
	return date;
}

bool Match::isPlayoffs() {
	return playoffs != NULL;
}
