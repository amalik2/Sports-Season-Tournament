#pragma once

#include "..\Teams\Team.h"

class TournamentMatchup
{

private:
	Team &lowerSeed;
	Team &higherSeed;

	int gamesWon[2] = { 0, 0 };	// the number of games each team has won
	int currentGame;	// the current game number in this series

public:

	// Amount of wins necessary to advance to the next round
	const static int REQUIRED_WINS = 2;
	const static int MAX_MATCHES = (REQUIRED_WINS * 2) - 1;

	TournamentMatchup(Team &lower, Team &higher);
	~TournamentMatchup();

	Team *getWinner();
	Team *getLoser();

	// The current match has finished
	// Returns whether this matchup is over or not
	bool matchFinished(Team *winner);

	Team &getHigherSeed();
	Team &getLowerSeed();
};

