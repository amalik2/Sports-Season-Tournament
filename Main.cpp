#include "Season.h"

#include <iostream>

int main() {

	Season s;

	for (int i = 0; i < 30; i++) {
		Team t("Team " + std::to_string(i));
		s.addTeam(t);
	}

	s.simulate();
	//s.start();

	std::vector<Team*> teams = s.getTeams(true, false);

	int i = 0;
	for (auto &team : teams) {
		//team->getRegSeasonStats().wins += i++;
		std::cout << team->getName() << " " << team->getWins() << " " << team->getLosses() << std::endl;
	}
	
	return 0;
}