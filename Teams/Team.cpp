#include "Team.h"

#include "..\Utilities\MathFunctions.h"

#include <tuple>

static int TEAM_INDEX = 0;

Team::Team(std::string tname)
{
	name = tname;
	index = TEAM_INDEX++;
	seed = 0;
	eliminated = false;
	division = -1;
}

Team::~Team()
{
}

std::string Team::getName()
{
	return name;
}

// return wins / (wins + losses)
float Team::getWinPercentage(bool tournament)
{
	if (tournament) {
		return MathFunctions::ratio(playoffStats.wins, playoffStats.wins + playoffStats.losses);
	}
	else {
		return MathFunctions::ratio(regStats.wins, regStats.wins + regStats.losses);
	}
}

void Team::eliminate()
{
	eliminated = true;
}

bool Team::isAliveInPlayoffs()
{
	// Playoffs have not started since no seed assigned
	if (seed == 0)
		return false;

	return !eliminated;
}

int Team::getIndex()
{
	return index;
}

int Team::getPlayoffSeed()
{
	return seed;
}

void Team::setPlayoffSeed(int s)
{
	seed = s;
}

int Team::getWins(bool playoffs)
{
	return playoffs ? playoffStats.wins : regStats.wins;
}

int Team::getLosses(bool playoffs)
{
	return playoffs ? playoffStats.losses : regStats.losses;
}

int Team::getTotalPoints(bool playoffs)
{
	return playoffs ? playoffStats.points : regStats.points;
}

void Team::increaseWins(bool playoffs)
{
	if (playoffs)
		playoffStats.wins++;
	else
		regStats.wins++;
}

void Team::increaseLosses(bool playoffs)
{
	if (playoffs)
		playoffStats.losses++;
	else
		regStats.losses++;
}

void Team::increaseTotalPoints(bool playoffs)
{
	if (playoffs)
		playoffStats.points++;
	else
		regStats.points++;
}

std::tuple<int, int, int> Team::getMatchupStats(Team & other)
{

	int total = 0;
	int wins = 0;
	int otherWins = 0;

	// TODO: test

	for (Match *& match : matches){
		if (&(match->getHomeTeam()) == &other || &(match->getAwayTeam()) == &other){
			total++;

			// Other team won
			if ((match->getWinner()) == &other){
				otherWins++;
			// This team won
			} else if (match->getWinner() != NULL){
				wins++;
			}
		}
	}
	

	return std::tuple<int, int, int>(total, wins, otherWins);
}

Match * Team::getMatchOnDay(DateTime date)
{
	for (Match *& match : matches) {
		if (!match->getDate().isSameDay(date)) {
			continue;
		}

		return match;
	}

	return NULL;
}

void Team::alterRecord(bool won)
{
	if (isAliveInPlayoffs()) {
		if (won)
			playoffStats.wins += 1;
		else
			playoffStats.losses += 1;
	}
	else {
		if (won)
			regStats.wins += 1;
		else
			regStats.losses += 1;
	}
}

const std::vector<Match*>& Team::getRegularMatchups() const {
	return matches;
}

void Team::addMatch(Match &match)
{
	matches.push_back(&match);
}

int Team::getCurrentPlayoffDivision()
{
	return division;
}

void Team::setCurrentPlayoffDivision(int newDiv)
{
	division = newDiv;
}
