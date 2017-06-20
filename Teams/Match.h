#pragma once

#include "Date/DateTime.h"

// Forward declaration of team
class Team;
class Tournament;

class Match
{

private:
	Team &home;
	Team &away;

	// Each team's points scored in this match
	int scores[2];

	// What round of the match is currently being played
	// 0 = match not started
	int round;

	// The date this match is played on
	DateTime date;

	// Pointer to playoffs object to inform it of a tournament matchup game being completed
	Tournament *playoffs;

public:
	Match(Team &h, Team &a, DateTime d);
	Match(Team &h, Team &a, DateTime d, Tournament *tourn);
	~Match();

	Team *getWinner();
	Team *getLoser();

	Team &getHomeTeam();
	Team &getAwayTeam();

	// Set the score for one of the two teams
	// home is set to true if the home team has it's score changed
	// Otherwise, the away team score is changed
	void setScore(int score, bool home);

	// Get the score for one of the two teams
	// home is set to true if the home team score is returned
	// Otherwise, the away team score is returned
	int getScore(bool home);

	// Begin the match
	// Returns whether the match was started or not
	bool start();

	// End the match
	// Returns whether the match was ended or not
	bool finish();

	// Get whether the match has finished or not
	bool isFinished();

	// Get the date this match will be played on
	DateTime getDate();

	// Get whether this is a playoff match or not
	bool isPlayoffs();

};

