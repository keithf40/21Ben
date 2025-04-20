#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "Deck.h"
#include "Dealer.h"
#include "Player.h"
#include "OptimalPlay.h"  
#include "Counter.h"
#include "memory"
#include <fstream>

class Simulation {
public:
    Simulation(int deckSize, int minBet, int startingMoney, int playerPosition, std::string playStyleOne);

    Simulation(int deckSize, int minBet, int startingMoney, int playerPosition, std::string playStyleOne, std::string otherStyle);

    std::vector<std::vector<long long>> Run(int handsDealt, int rounds);

private:
    Dealer dealer;
    std::unique_ptr<Deck> deck;
    std::unique_ptr<Counter> counterOne;
    std::unique_ptr<Counter> counterTwo;
    std::vector<float> betRatios{ std::vector<float>(5) };
    std::vector<long long> balances;
    std::vector<Player> players;  //All bots
    OptimalPlay optimal;

    // Human player index is now 4.
    int playerPosition = 4;
    bool competingCounts = false;
    int minBet = 15;
    int startingMoney = 100;
    bool sleuth = true;
    void initializeRound();

    void playerTurn(Player& player);
};

