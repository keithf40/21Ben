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

std::vector<std::vector<long long>> Simulation::Run(int handsDealt, int rounds) {
	for (int i = 0; i < 5; i++) {
		std::string name = "Position" + std::to_string(i + 1);
		Player foo(name, startingMoney);
		players.push_back(foo);
	}

	/*std::ofstream outfile("simulation.txt");
	if (outfile.is_open()) {
		outfile << counterOne->getStrategy();
		if (competingCounts) {
			outfile << " " << counterTwo->getStrategy();
		}
		outfile << "\n";
	}
	outfile.close();*/
	std::vector<long long>foo;
	std::vector<std::vector<long long>> finalStats(2, foo);
	finalStats[0].push_back(rounds);
	finalStats[0].push_back(handsDealt);
	std::vector<long long>totals(5, 0);
	finalStats.push_back(totals);
	finalStats.push_back(totals);

	for (int i = 0; i < rounds; i++) {
		deck->shuffle();
		counterOne->resetCount();
		counterTwo->resetCount();
		balances.clear();
		for (int j = 0; j < players.size(); j++) {
			players[j].setBank(startingMoney);
			players[j].reset();
			balances.push_back(startingMoney);
		}

		for (int j = 0; j < handsDealt; j++) {
			std::vector<long long> balanceDiff;
			for (auto p : players) {
				balanceDiff.push_back(p.getBalance());
			}

			dealer.clear();
			if (deck->shuffleReady()) {
				deck->shuffle();
				counterOne->resetCount();
				counterTwo->resetCount();
			}
			int counterOneCount = counterOne->getCount() / deck->getDecksRemaining();
			int counterTwoCount = counterTwo->getCount() / deck->getDecksRemaining();

			initializeRound();
			for (int j = 0; j < players.size(); j++) {
				for (int i = 0; i < players[j].getTotalHands(); i) {
					Card dealerUp = dealer.getHand().getCards()[1];
					char move = optimal.getMove(players[j].getCurrentHand(), dealerUp);
					if (move == 'H') {
						Card count = players[j].hit(*deck, *counterOne);
						counterTwo->modifyCount(count);
					}
					else if (move == 'S') {
						i++;
						players[j].advanceHand();
					}
					else if (move == 'D') {
						Card count = players[j].hit(*deck, *counterOne);
						counterTwo->modifyCount(count);
						if (players[j].doubleDown()) {
							i++;
							players[j].advanceHand();
						}
					}
					else if (move == 'Y') {
						players[j].split();
					}
					else if (move == 'B') {
						i++;
						players[j].advanceHand();
					}
				}
			}

			dealer.dealerTurn(*deck, *counterOne);
			auto dealerCards = dealer.getHand().getCards();

			for (int i = 2; i < dealerCards.size(); i++) {
				counterTwo->modifyCount(dealerCards[i]);
			}

			int dealerScore = dealer.getHand().getTotalValue();
			bool dealerBlackjack = dealer.checkBlackjack();
			int k = 1;

			/*std::ofstream outfile("simulation.txt", std::ios::app);*/
			
			long long netGain = players[0].totalWinnings(dealerScore, dealerBlackjack);
			players[0].addWinnings(netGain);
			balanceDiff[0] = players[0].getBalance() - balanceDiff[0];
			balances[0] += balanceDiff[0] * betRatios[0];
			if (true) {
				//outfile << std::to_string(players[0].getBalance());
				
				for (int i = 1; i < players.size(); i++) {
					netGain = players[i].totalWinnings(dealerScore, dealerBlackjack);
					players[i].addWinnings(netGain);
					balanceDiff[i] = players[i].getBalance() - balanceDiff[i];
					balances[i] += betRatios[i] * balanceDiff[i];
					/*outfile << ",";
					outfile << players[i].getBalance();*/
					k++;
				}
				
				//outfile << "\n";
				//outfile << std::to_string(balances[0]);
				/*for (int i = 1; i < balances.size(); i++) {
					outfile << ",";
					outfile << std::to_string(balances[i]);
				}
				outfile << " Count was ";
				outfile << counterTwoCount;
				outfile << "\n";*/
				//outfile.close();
			}
			balanceDiff.clear();
			
		}
		/*std::ofstream outfile("simulation.txt", std::ios::app);
		outfile << "End of simulation ";
		outfile << i + 1;
		outfile << "\n";
		outfile.close();*/
		
		for (int i = 0; i < finalStats[2].size(); i++) {
			finalStats[2][i] += players[i].getBalance() - startingMoney;
			finalStats[3][i] += balances[i] - startingMoney;
		}
	}
	for (int i = 0; i < finalStats[2].size(); i++) {
		finalStats[2][i] /= rounds;
		finalStats[3][i] /= rounds;
	}
	return finalStats;
}
//we dont need to do this but I felt like putting them in separate functiong might make the code
//more readable, just theory crafting readable code thats all
void Simulation::initializeRound() {
	
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
	
}
