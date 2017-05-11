#include "TournamentRound.h"

#include <iostream>
#include <algorithm>

#include "..\Season.h"

TournamentRound::TournamentRound()
{
	finishedMatchups = -1;
}

TournamentRound::~TournamentRound()
{
}

// one > two if true
// one < two is better
bool compareDivisions(Team *one, Team *two) {
	// TODO: test

	if (one->getCurrentPlayoffDivision() != two->getCurrentPlayoffDivision())
		return one->getCurrentPlayoffDivision() < two->getCurrentPlayoffDivision();

	return Team::compareTeams(one, two);//one->getPlayoffSeed() < two->getPlayoffSeed();
}

void TournamentRound::createMatchups(std::vector<Team*>& teams)
{
	// Assign new divisions to each team (Can be anywhere from 1 to the number of teams / 2)
	for (int i = 0; i < teams.size(); i++) {
		int div = teams[i]->getCurrentPlayoffDivision();

		// No division has been set, so use the team's seed
		if (div == -1) {
			div = teams.at(i)->getPlayoffSeed();
			teams[i]->setCurrentPlayoffDivision(div);
		}

		// If the division is less than or equal to half the number of teams, then this will be the team's division
		if (div <= teams.size() / 2) {
			continue;
		}

		// Assign a new division, based on how far away the current division is from the total number of divisions in this round
		// The team with the division that is currently the highest number will face off against the team with the lowest division
		int newDiv = div - (teams.size() / 2);
		newDiv = ((teams.size() / 2) + 1) - newDiv;
		teams.at(i)->setCurrentPlayoffDivision(newDiv);

		 //std::cout << "OLD DIVISION = " << div << " NEW DIVISION = " << newDiv << "SEED = " << teams.at(i)->getPlayoffSeed() << " --> " << teams.at(i)->getName() << std::endl;
	}

	// Sort, so that teams with the same divisions are beside each other
	std::sort(teams.begin(), teams.end(), compareDivisions);

	// Create matchups for this round
	for (int i = 0; i < teams.size(); i += 2) {
		TournamentMatchup matchup(*teams[i], *teams[i + 1]);
		//std::cout << " TEAMS: " << teams.at(i)->getName() << " VS. " << teams.at(i + 1)->getName() << std::endl;
		matchups.push_back(matchup);
	}
	finishedMatchups = 0;

}

void TournamentRound::start(std::vector<Team*>& teams)
{
	createMatchups(teams);
}


bool TournamentRound::teamWonMatchup(Team & team)
{
	for (TournamentMatchup &matchup : matchups) {
		if (matchup.getHigherSeed() == team || matchup.getLowerSeed() == team) {
			finishedMatchups++;
		}
	}

	// If all of the matchups in this round have finished, move to the next round
	return hasFinished();
}

TournamentMatchup * TournamentRound::getMatchup(Team * team)
{
	for (TournamentMatchup &matchup : matchups) {

		// If one of the teams in this matchup is the specified team
		if (matchup.getHigherSeed() == *team || matchup.getLowerSeed() == *team) {
			return &matchup;
		}
	}
	return NULL;
}

bool TournamentRound::hasFinished()
{
	return matchups.size() == finishedMatchups;
}

std::vector<TournamentMatchup>& TournamentRound::getMatchups()
{
	return matchups;
}
