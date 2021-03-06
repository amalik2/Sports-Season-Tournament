#pragma once

#include <string>
#include <vector>
#include <tuple>

#include "Match.h"

struct TeamStats {
	int wins = 0;
	int losses = 0;
	int points = 0;		// total points scored
};

class Team
{

private:
	std::string name;

	// Regular season statistics
	TeamStats regStats;
	// Playoffs statistics
	TeamStats playoffStats;
	// Eliminated from the playoffs if this is set to true
	bool eliminated;

	// Unique team index
	int index;

	// Playoff seed
	int seed;
	// Playoff division
	int division;

	// Pointers to an element in the current season match list
	std::vector<Match*> matches;

public:
	Team(std::string tname);
	~Team();

	std::string getName();

	// Get the percentage of wins to total games played
	// If tournament is set to true, use stats from the playoffs (regular season otherwise)
	float getWinPercentage(bool tournament);

	// Eliminate this team from the playoffs
	void eliminate();

	// Get whether the team is currently in the playoffs
	// Always returns false if the playoffs have not started
	bool isAliveInPlayoffs();

	// Get the team's unique index
	int getIndex();

	int getPlayoffSeed();
	void setPlayoffSeed(int s);

	int getWins(bool playoffs = false);
	int getLosses(bool playoffs = false);
	int getTotalPoints(bool playoffs = false);

	void increaseTotalPoints(bool playoffs = false);

	// Return (Total matchups, # games this team won, # games other team won)
	std::tuple<int, int, int> getMatchupStats(Team &other);

	// Return the match that this team plays on the specified date
	// Returns null if the team has no match on that date
	Match *getMatchOnDay(DateTime date);

	// Alter the team record
	// if won is true, then adds a win. Otherwise, add a loss
	void alterRecord(bool won);

	bool operator== (Team *other) {
		if (other == NULL)
			return false;

		return getIndex() == other->getIndex();
	}

	bool operator== (Team &other) {
		return getIndex() == other.getIndex();
	}

	const std::vector<Match*> &getRegularMatchups() const;

	// Add a new match to the schedule
	void addMatch(Match &match);

	int getCurrentPlayoffDivision();
	void setCurrentPlayoffDivision(int newDiv);

	// Compare functor. Lower seed = better
	// returns true if team one is before team two
	static bool compareTeams(Team *&one, Team *&two);

};

