#include "Season.h"

#include <math.h>
#include <algorithm>
#include <random>
#include <chrono>
#include <map>

#include <iostream>
#include <exception>

#include "Utilities\RandomFunctions.h"

static const int MAX_TEAMS = 300;
static const int GAMES_PLAYED = 160;
/*// The maximum number of matches that can be on at the same time
const int MAX_CONCURRENT_GAMES = 1;

// The maximum number of times a team will play another team in the same conference as them
const int MAX_IN_CONFERENCE = 3;

// The number of games each team will play against teams in the other conference
const int OTHER_CONFERENCE_GAMES = (GAMES - (((TEAMS / 2) - 1) * MAX_IN_CONFERENCE)) / ((TEAMS / 2));

// The maximum amount of times one team will play the same team
const int MAX_GAMES_SAME_OPPONENT = 2;

// The maximum number of back to back matches a team can have
const int MAX_BACK_TO_BACK = 2;
*/

// The earliest a match can start at: 4 PM
const int MINIMUM_START_HOUR = 16;

// The latest a match can start at: 10 PM
const int MAXIMUM_START_HOUR = 22;

// The date for the first game of the season
static Date SEASON_START(1180, 10, 30);

// The date for the final game of the season
static Date SEASON_END(1181, 2, 9);

// The date for the final playoff game of the season
static Date PLAYOFFS_END(1181, 3, 24);

// Number of hours a game will start after the previous one has started
const int HOURS_BETWEEN_GAMES = 2;

// Games per day cycle
const int GPD_LENGTH = 6;
const int GAMES_PER_DAY[GPD_LENGTH] = { 3, 2, 3, 2, 1, 3 };

Season::Season()
{
	seasonStatus = OFFSEASON;

	// Reserve size to prevent pointer invalidation when new elements are added
	teams.reserve(MAX_TEAMS);
	matches.reserve(MAX_TEAMS * GAMES_PLAYED);
}

Season::~Season()
{
}

void Season::start()
{

	if (teams.size() % 2 == 1) {
		std::cout << "Even number of teams required. An odd number of teams were registered. Exiting" << std::endl;
		throw std::exception("There must be an even number of teams to avoid scheduling problems");
	}

	if (teams.size() == 0) {
		std::cout << "No teams were registered. Exiting" << std::endl;
		throw std::exception("No teams registered");
	}

	if (seasonStatus == OFFSEASON) {
		seasonStatus = REGULAR;
		createSchedules(false, SEASON_START);
	}
}

void Season::simulate()
{
	start();
	Date d = SEASON_START;

	while (d.isBefore(SEASON_END)) {
		for (Match &match : matches) {

			if (match.isFinished()) {
				if (&match == &matches.back()) {
					d = SEASON_END;
					//std::cout << " FINISHED REGULAR SEASON " << std::endl;
					break;
				}
				continue;
			}

			d.setTime(match.getDate().getHours(), 0, 0);
			if (match.getDate().isTimeForEvent(d)) {
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
			else {
				d.addDays(1);
				break;
			}
		}
	}
	//std::cout << " FINISHED REGULAR SEASON 2 " << std::endl;
	increaseSeasonStatus();

	while (true) {
		for (Match &match : playoffMatches) {
			if (match.isFinished() || !match.getAwayTeam().isAliveInPlayoffs() || !match.getHomeTeam().isAliveInPlayoffs())
				continue;

			d = match.getDate();

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

		if (playoffTournament.getWinner() != NULL) {
			break;
		}

		/*int rnd = playoffTournament.getCurrentRoundNumber();

		for (TournamentMatchup &m : playoffTournament.getCurrentRound()->getMatchups()) {
			std::cout << m.getHigherSeed().getName() << " vs. " << m.getLowerSeed().getName() << " NULLWIN? " << (m.getWinner() == NULL) << std::endl;
		}*/

		nextPlayoffRound(d);
		increaseSeasonStatus();

	}

	increaseSeasonStatus();
}

void Season::startPlayoffs()
{
	createPlayoffSeeds();
	std::vector<Team*> activeTeams = getTeams(true, true);
	playoffTournament.start(activeTeams);
	createSchedules(true, SEASON_END);
}

bool Season::nextPlayoffRound(Date d)
{

	int rnd = playoffTournament.getCurrentRoundNumber();
	if (rnd == 0 || rnd > getTotalPlayoffRounds()) {
		return false;
	}

	// Add an extra day to prevent matches from previous round being on same day
	d.addDays(1);
	createSchedules(true, d);

	return true;
}

// TODO
void Season::finish()
{
	std::cout << "The season has finished. The winner is: " << getChampion()->getName() << std::endl;
}

// 2
// 3
// 1 round: 3 games

// 4
// 3
// 2 rounds: 9 games

//8
//3
//3 rounds: 12 (round 1) + 6 (round 2) + 3 (round 4) = 21

//16
//3
//4 rounds: 24 (round 1) + 12 (round 2) + 6 (round 3) + 3 (round 4) = 45
int Season::getTotalPlayoffGames()
{
	int amt = 0;
	int rounds = getTotalPlayoffRounds();
	int teamsInRound = getMaxTeamsInPlayoffs();
	// TODO: test
	while (rounds > 0) {
		teamsInRound /= 2;
		amt += teamsInRound * TournamentMatchup::MAX_MATCHES;
		rounds -= 1;
	}

	return amt;

}

int Season::getPlayoffRound()
{
	if (!hasPlayoffsStarted())
		return 0;
	return seasonStatus;
}

bool Season::hasPlayoffsStarted()
{
	return seasonStatus > REGULAR;
}

int Season::getSeasonStatus()
{
	return seasonStatus;
}

void Season::setSeasonStatus(int s)
{
	seasonStatus = s;

	if (seasonStatus == REGULAR + 1)
		startPlayoffs();
	else if (seasonStatus == getTotalPlayoffRounds() + 1)
		finish();

}

void Season::increaseSeasonStatus()
{
	setSeasonStatus(seasonStatus + 1);
}

int Season::getTotalPlayoffRounds() {

	if (teams.size() <= 2)
		return (int)floor(teams.size() / 2);

	float base = log(teams.size() / 2) / log(2);
	// Not enough teams, so return the floor instead
	if (pow(2, ceil(base)) > teams.size()) {
		return (int)floor(base);
	}

	return (int)ceil(base);
}

int Season::getMaxTeamsInPlayoffs()
{
	return (int)pow(2, getTotalPlayoffRounds());
}

void Season::createSchedules(bool playoffs, Date date) {

	if (playoffs) {
		playoffMatches.reserve(getTotalPlayoffGames());
	}

	std::vector<Team*> teamptrs = getTeams(false, playoffs);

	/*if (playoffs)
		std::cout << "ACTIVE TEAMS = " << teamptrs.size() << std::endl;
		*/

	std::vector<Match> &matchList = playoffs ? playoffMatches : matches;

	// Shuffle the list randomly (using the current time), so that the order of the matchups will vary
	auto randomEngine = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
	std::shuffle(teamptrs.begin(), teamptrs.end(), randomEngine);

	const int amtTeams = teamptrs.size();

	// The date for the current match
	date.setTime(MINIMUM_START_HOUR, 0, 0);
	//int daysInSeason = date.getNumberDaysBefore(playoffs ? PLAYOFFS_END : SEASON_END);

	int gpdIndex = 0;

	// The number of matches that will be played on this day
	int matchesRemaining = playoffs ? (1 + getTotalPlayoffRounds() - playoffTournament.getCurrentRoundNumber()) : GAMES_PER_DAY[gpdIndex];

	int gamesPerTeam = playoffs ? TournamentMatchup::MAX_MATCHES : GAMES_PLAYED;

	// Per team. Only used for playoffs
	std::map<int, int> playoffMatchesMade;

	// Playoffs: Create the specified amount of games for playoff matchups
	// Non Playoffs: Create the specified amount of games using the Round Robin algorithm
	for (int i = 1; i <= gamesPerTeam; i++) {
		// Playoffs: For each team
		// Non Playoffs: For each team in the first half of the list
		for (int t = 0; t < (playoffs ? amtTeams : (amtTeams / 2)); t++) {

			Team *team1 = NULL, *team2 = NULL;

			if (playoffs) {

				team1 = teamptrs[t];

				// Enough matches made for this team already
				if (playoffMatchesMade[team1->getIndex()] >= gamesPerTeam) {
					continue;
				}

				team2 = &playoffTournament.getCurrentRound()->getMatchup(team1)->getHigherSeed();
				if (team1->getIndex() == team2->getIndex())
					team2 = &playoffTournament.getCurrentRound()->getMatchup(team1)->getLowerSeed();

				playoffMatchesMade[team1->getIndex()]++;
				playoffMatchesMade[team2->getIndex()]++;

			}
			else {
				team1 = teamptrs[(amtTeams - t) % amtTeams];
				// Face the opponent that is in the corresponding position on the other half of the list
				team2 = teamptrs[(amtTeams + t + 1) % amtTeams];

				// If either team has enough matches, skip this matchup
				if (team1->getRegularMatchups().size() >= GAMES_PLAYED || team2->getRegularMatchups().size() >= GAMES_PLAYED)
					continue;
			}

			// Create matchup
			if (playoffs) {
				Match match(*team1, *team2, date, &playoffTournament);
				matchList.push_back(match);
			}
			else {
				Match match(*team1, *team2, date);
				matchList.push_back(match);
			}
			team1->addMatch(matchList.back());
			team2->addMatch(matchList.back());

			matchesRemaining--;
			// Update the time that the next game will start at
			if (date.getHours() + HOURS_BETWEEN_GAMES <= MAXIMUM_START_HOUR) {
				date.setTime(date.getHours() + HOURS_BETWEEN_GAMES, 0, 0);
			}
			else {
				matchesRemaining = 0;
			}

			if (matchesRemaining == 0) {
				// Update date to the next day, starting at a random hour
				date.addDays(1);
				date.setTime(MINIMUM_START_HOUR + RandomFunctions::randint(0, 2), 0, 0);

				// Change the index for the number of games for the next day
				gpdIndex = (gpdIndex + 1) % GPD_LENGTH;
				matchesRemaining = playoffs ? (1 + getTotalPlayoffRounds() - playoffTournament.getCurrentRoundNumber()) : GAMES_PER_DAY[gpdIndex];
			}
		}
		// Rotate the vector so that the first element remains the same, but all of the other elements are shifted to the left by one position
		if (!playoffs)
			std::rotate(teamptrs.begin() + 1, teamptrs.begin() + 2, teamptrs.end());
	}

	/*for (Match &match : matchList) {
		std::cout << match.getDate().getDisplayDate() << match.getAwayTeam().getName() << " vs. " << match.getHomeTeam().getName() << std::endl;
	}
	*/
	std::cout << "Total = " << matchList.size() << std::endl;
	
}

Team *Season::getTeamByIndex(int index)
{
	/*// Random access check first (useful before the playoffs)
	if (teams[index].getIndex() == index) {
		return &teams[index];
	}
	// Iterate through teams to find the correct one
	for (Team &team : teams) {
		if (team.getIndex() == index)
			return &team;
	}

	return NULL;*/
	return &teams[index];
}

void Season::createPlayoffSeeds()
{

	std::vector<Team*> sortedTeams = getTeams(true, false);

	int index = 0;
	for (Team *&team : sortedTeams) {
		if (++index > getMaxTeamsInPlayoffs()) {
			team->eliminate();
			continue;
		}
		team->setPlayoffSeed(index);
	}
}

bool Season::addTeam(Team team)
{
	// Add only if the season has not started yet
	if (seasonStatus == OFFSEASON && teams.size() < MAX_TEAMS) {
		teams.push_back(team);
		return true;
	}

	return false;
}

std::vector<Team*> Season::getTeams(bool sorted, bool onlyUneliminated)
{

	std::vector<Team*> teamptrs;

	// Add pointers for each team
	for (auto &team : teams) {
		if (!onlyUneliminated || team.isAliveInPlayoffs())
			teamptrs.push_back(&team);
	}

	if (sorted)
		std::sort(teamptrs.begin(), teamptrs.end(), Team::compareTeams);

	return teamptrs;
}

Team * Season::getChampion()
{
	return playoffTournament.getWinner();
}

