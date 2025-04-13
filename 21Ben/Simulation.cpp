#include "Simulation.h"

Simulation::Simulation(int deckSize, int minBet, int startingMoney, int playerPosition, std::string playStyleOne) {
	deck = std::make_unique<Deck>(deckSize);
	this->minBet = minBet;
	this->startingMoney = startingMoney;
	this->playerPosition = playerPosition;

	counterOne = std::make_unique<Counter>(playStyleOne);
	counterTwo = std::make_unique<Counter>(playStyleOne);
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
	std::vector<std::vector<long long>> counter1Results;
	std::vector<std::vector<long long>> counter2Results;
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

			dealer.dealerTurn(*deck, *counterOne);
			auto dealerCards = dealer.getHand().getCards();
			
			for (int i = 2; i < dealerCards.size(); i++) {
				counterTwo->modifyCount(dealerCards[i]);
			}

			std::vector<long long> counter1;
			std::vector<long long> counter2;
			
			int dealerScore = dealer.getHand().getTotalValue();
			bool dealerBlackjack = dealer.checkBlackjack();
			int k = 0;
			
			for (auto p : players) {
				long long netGain = p.totalWinnings(dealerScore, dealerBlackjack);
				p.addWinnings(netGain);
				balances[k] += netGain * betRatios[k];
				counter1.push_back(netGain);
				counter2.push_back(balances[k]);
				k++;
			}
			counter1Results.push_back(counter1);
			counter2Results.push_back(counter2);

		}
	}
	std::ofstream outfile("simulation.txt");

	if (outfile.is_open()) {
		outfile << counterOne->getStrategy();
		if (competingCounts) {
			outfile << " " << counterTwo->getStrategy();
		}
		outfile << "\n";
		for (int i = 0; i < counter1Results.size(); i++) {
			outfile << counter1Results[i][0];
			for (int j = 0; j < counter1Results[i].size(); j++) {
				outfile << ", " << counter1Results[i][j];

			}
			outfile << "\n";
			if (competingCounts) {
				outfile << counter2Results[i][0];
				for (int j = 0; j < counter2Results[i].size(); j++) {
					outfile << ", " << counter2Results[i][j];
				}
				outfile << "\n";
			}
		}
		outfile.close();
	}
}
//we dont need to do this but I felt like putting them in separate functiong might make the code
//more readable, just theory crafting readable code thats all
void Simulation::initializeRound() {
	dealer.clear();
	if (deck->shuffleReady()) {
		deck->shuffle();
		counterOne->resetCount();
		counterTwo->resetCount();
	}
	for (int i = 0; i < players.size(); i++) {
		players[i].reset();
		players[i].placeBet(counterOne->getBet(deck->getDecksRemaining(),
			players[i].getBalance(), minBet, sleuth));
		float otherBet = counterTwo->getBet(deck->getDecksRemaining(),
			balances[i], minBet, sleuth);
		betRatios[i] = otherBet / players[i].getCurrentBet();
	}
	
	dealer.hit(*deck, *counterOne, true);
	Card card = dealer.hit(*deck, *counterOne, false);
	counterTwo->modifyCount(card);
	
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
	for (int i = 0; i < player.getTotalHands(); i) {
		Card dealerUp = dealer.getHand().getCards()[1];
		char move = optimal.getMove(player.getCurrentHand(), dealerUp);
		if (move == 'H') {
			Card count = player.hit(*deck, *counterOne);
			counterTwo->modifyCount(count);
		}
		else if (move == 'S') {
			i++;
			player.advanceHand();
		}
		else if (move == 'D') {
			Card count = player.hit(*deck, *counterOne);
			counterTwo->modifyCount(count);
			if (player.doubleDown()) {
				i++;
				player.advanceHand();
			}
		}
		else if (move == 'Y') {
			player.split();
		}
	}
}
