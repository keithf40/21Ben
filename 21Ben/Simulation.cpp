#include "Simulation.h"

Simulation::Simulation(int deckSize, int minBet, int startingMoney, int playerPosition, std::string playStyleOne) {
	deck = std::make_unique<Deck>(deckSize);
	this->minBet = minBet;
	this->startingMoney = startingMoney;
	this->playerPosition = playerPosition;

	counterOne = std::make_unique<Counter>(playStyleOne);
}

Simulation::Simulation(int deckSize, int minBet, int startingMoney, int playerPosition,
										std::string playStyleOne, std::string otherStyle) {
	deck = std::make_unique<Deck>(deckSize);
	this->minBet = minBet;
	this->startingMoney = startingMoney;
	this->playerPosition = playerPosition;

	counterOne = std::make_unique<Counter>(playStyleOne);
	counterTwo = std::make_unique<Counter>(otherStyle);
	competingCounts = true;
}

void Simulation::Run(int handsDealt, int rounds) {
	Dealer dealer;
	for (int i = 0; i < 5; i++) {
		std::string name = "Position" + std::to_string(i + 1);
		Player foo(name, startingMoney);
		players.push_back(foo);
	}

	for (int i = 0; i < rounds; i++) {
		deck->shuffle();
		counterOne->resetCount();
		for (int j = 0; j < players.size(); j++) {
			players[j].setBank(startingMoney);
			players[j].reset();
		}
		
		for (int j = 0; j < handsDealt; j++) {
			initializeRound();
			for (auto p : players) {
				playerTurn(p);
			}
		}
	}
}
//we dont need to do this but I felt like putting them in separate functiong might make the code
//more readable, just theory crafting readable code thats all
void Simulation::initializeRound() {
	dealer.clear();
	for (int i = 0; i < players.size(); i++) {
		players[i].reset();
	}
	if (deck->shuffleReady()) {
		deck->shuffle();
		counterOne->resetCount();
		counterTwo->resetCount();
	}
	for (int i = 0; i < players.size(); i++) {
		players[i].placeBet(optimal.getBet(counterOne->getCount(), deck->getDecksRemaining(), 
							players[i].getBalance(),minBet, sleuth));
		//TODO: Create a bet vector that holds information about the second counters bets including
		//The values that would be bet on a split hand, so will be a 2d vector, only workaround I see for this
		//Without changing the code a lot or being odd
	}
	dealer.hit(*deck, *counterOne, true);
	Card card = dealer.hit(*deck, *counterOne, false);
	if (competingCounts) {
		counterTwo->modifyCount(card);
	}
	for (int pos = 0; pos < players.size(); pos++) {
		Card first = players[pos].hit(*deck, *counterOne);
		Card second = players[pos].hit(*deck, *counterOne);
		if (competingCounts) {
			counterTwo->modifyCount(first);
			counterTwo->modifyCount(second);
		}
	}
}

void Simulation::playerTurn(Player& player) {
	for (int i = 0; i < player.getTotalHands(); i++) {

	}
}
