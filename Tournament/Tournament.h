#pragma once

#include <vector>

#include "TournamentRound.h"

class Tournament
{

private:
	std::vector<TournamentRound> rounds;

	std::vector<Team*> activeParticipants;

	// Begin the next round
	void newRound();

	// Call this when the current round has been finished
	void roundFinished(TournamentRound &round);

public:

	Tournament();
	~Tournament();

	// Begin the tournament with the given teams list
	void start(std::vector<Team*> teams);

	// Get the current round of this tournament
	// Returns null if it has not started yet
	TournamentRound *getCurrentRound();

	// Return the number of the current round
	int getCurrentRoundNumber();

	// A team in the tournament has won a game. Update tournament to reflect this
	void teamWonGame(Team *winner);

	// Return the winner of the tournament if there is one
	Team *getWinner();

};

