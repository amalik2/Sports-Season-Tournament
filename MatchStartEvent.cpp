#include "MatchStartEvent.h"

#include "Utilities/RandomFunctions.h"

#include "Teams/Team.h"

MatchStartEvent::MatchStartEvent(Match &matchRef, DateTime start) : CalendarEvent(start), match(matchRef)
{
}


MatchStartEvent::~MatchStartEvent()
{
}

void MatchStartEvent::handleImplementation() {

	// one of the two teams was knocked out already, so this match should not be played
	if (match.isPlayoffs() && (!match.getAwayTeam().isAliveInPlayoffs() || !match.getHomeTeam().isAliveInPlayoffs()))
		return;

	// match simulation
	int score1 = RandomFunctions::randint(0, 7);
	int score2 = RandomFunctions::randint(1, 6);
	while (score1 == score2) {
		score2++;
	}
	match.start();
	match.setScore(score1, true);
	match.setScore(score2, false);
	match.finish();
}
