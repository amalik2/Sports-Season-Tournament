#pragma once

#include <vector>

#include "..\Teams\Team.h"
#include "TournamentMatchup.h"

class TournamentRound
{

private:
	std::vector<TournamentMatchup> matchups;
	// The number of matchups that have been finished
	int finishedMatchups;

	// Assign matchups for each team
	void createMatchups(std::vector<Team*> &teams);

public:
	TournamentRound();
	~TournamentRound();

	// Start this round of the playoffs with the given teams as participants
	void start(std::vector<Team*> &teams);

	// Inform this object that a team has won their matchup
	// Returns whether this round is complete or not
	bool teamWonMatchup(Team &team);

	// Get the matchup that the given team is involved in
	TournamentMatchup *getMatchup(Team *team);

	// Get whether this round has finished or not
	bool hasFinished();

	// Return the full list of matchups
	std::vector<TournamentMatchup> &getMatchups();

};

