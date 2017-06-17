#pragma once

#include <vector>
#include <tuple>

#include "Tournament\Tournament.h"

class Season
{

private:
	// Represents what state the season is currently in (offseason, regular season, playoffs, finished)
	int seasonStatus;

	std::vector<Team> teams;
	std::vector<Match> matches;
	std::vector<Match> playoffMatches;
	
	Tournament playoffTournament;

	// Get the number of teams that will play in the first round of the playoffs
	int getMaxTeamsInPlayoffs();

	// Create the schedules for all teams
	// date is the start date for the matches
	void createSchedules(bool playoffs, Date date);

	// Get a team by their unique index (>= 0)
	// Undefined behaviour if index is invalid
	Team *getTeamByIndex(int index);

	// Assign playoff seeds to all teams, and eliminate those failing to make the playoffs
	void createPlayoffSeeds();

	// Begin the season playoffs
	void startPlayoffs();

	// End the season playoffs
	void finish();

	// Get the total number of games that will be played in the playoffs
	int getTotalPlayoffGames();

	// Start the next round of the playoffs
	// Returns whether the next round was started or not
	bool nextPlayoffRound(Date d);

	// Change the season status code to s
	void setSeasonStatus(int s);

	// Increase the season status code by 1
	void increaseSeasonStatus();

public:

	// Season status codes
	static const int OFFSEASON = -1, REGULAR = 0;

	Season();
	~Season();

	// Officially start this season
	// REQUIRES AN EVEN NUMBER OF TEAMS TO BE REGISTERED, AND ATLEAST 2 MUST BE REGISTERED
	// std::exception will be thrown if those conditions are not met
	void start();

	// Simulate the entire season
	void simulate();

	///////////////SEASON STATUS/////////////////////

	// Returns the current playoff round number
	// Returns 0 if the playoffs have not yet started
	int getPlayoffRound();

	// Get whether the playoffs have started or not
	bool hasPlayoffsStarted();

	// Gets the current status code of the season
	// -1 = Season has not started
	// 0 = Season is under progress
	// X (>0) = Season is in round X of the playoffs
	int getSeasonStatus();

	// Get the total number of rounds in the playoffs
	int getTotalPlayoffRounds();

	///////////////TEAMS/////////////////////

	// Add a new team to the competitors if the season has not yet started
	// Return whether the team was added or not
	bool addTeam(Team team);

	// Return a vector of pointers to all teams
	// if sorted is true, return them in sorted order
	// if onlyUneliminated is true, only teams that have not been eliminated from the playoffs will be returned
	std::vector<Team*> getTeams(bool sorted, bool onlyUneliminated);
	
	// Get the team that won this season's playoffs if it is over
	Team *getChampion();

};

