#include "Tournament.h"

#include "..\Utilities\VectorFunctions.h"

Tournament::Tournament()
{
}


Tournament::~Tournament()
{
}

void Tournament::start(std::vector<Team*> teams)
{

	// Already started
	if (activeParticipants.size() != 0)
		return;

	int amtRounds = log(teams.size()) / log(2);
	for (int i = 0; i < amtRounds; i++) {
		TournamentRound r;
		rounds.push_back(r);
	}
	activeParticipants = teams;

	if (amtRounds != 0)
		rounds[0].start(activeParticipants);
	else {
		// Tournament already finished since only one team was in it
		TournamentRound r;
		rounds.push_back(r);
		roundFinished(rounds.back());
	}
}

TournamentRound * Tournament::getCurrentRound()
{
	if (activeParticipants.size() == 0)
		return NULL;

	for (TournamentRound &r : rounds) {
		if (!r.hasFinished()) {
			return &r;
		}
	}

	return NULL;
}

int Tournament::getCurrentRoundNumber()
{
	if (activeParticipants.size() == 0)
		return 0;

	for (int i = 0; i < rounds.size(); i++) {
		if (!rounds[i].hasFinished()) {
			return i + 1;
		}
	}

	return 0;
}

// TODO
void Tournament::newRound()
{
	//std::cout << "New Round Started: " << getCurrentRoundNumber() << std::endl;
	getCurrentRound()->start(activeParticipants);
}

void Tournament::roundFinished(TournamentRound & round)
{
	//std::cout << "TOURNY ROUND FINISHED" << std::endl;

	// The only remaining active participant is the winner
	// TODO
	if (&round == &rounds.back()) {

	}
	else {
		newRound();
	}

}

void Tournament::teamWonGame(Team * winner)
{
	if (getCurrentRound()->getMatchup(winner)->matchFinished(winner)) {

		//std::cout << "WINNER MATCHUP: " << winner->getName() << std::endl;
		getCurrentRound()->getMatchup(winner)->getLoser()->eliminate();
		removeFromVector(activeParticipants, getCurrentRound()->getMatchup(winner)->getLoser());

		TournamentRound &currentRound = *getCurrentRound();
		if (currentRound.teamWonMatchup(*winner)) {
			//std::cout << "ROUND OVER: " << std::endl;
			roundFinished(currentRound);
		}
	}
}

Team * Tournament::getWinner()
{
	return activeParticipants.size() == 1 ? activeParticipants[0] : NULL;
}
