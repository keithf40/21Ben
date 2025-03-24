#pragma once
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "Deck.h"
#include "Dealer.h"
#include "Player.h"

class Game {
public:
    enum class Option { SKIP, NONE };

    Game(float width, float height);

    // Draw the game screen including player hand, dealer hand, messages, and action buttons.
    void draw(sf::RenderWindow& window);
    // Handle events including mouse clicks for game actions.
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);

    Option getSelectedOption() const;

    // Start a new round.
    void startNewRound();

private:
    sf::Font font;
    std::vector<sf::Text> menuOptions;  // For the Skip button.
    int selectedMenuIndex = 0;

    // Action buttons for gameplay (Hit and Stand).
    sf::Text hitButton;
    sf::Text standButton;
    bool hitHovered = false;
    bool standHovered = false;

    // Core game components.
    Deck deck;
    Dealer dealer;
    Player player;

    bool roundInProgress;
    std::string message;

    // Text objects for display.
    sf::Text playerHandText;
    sf::Text dealerHandText;
    sf::Text messageText;

    // Update display texts.
    void updateDisplayText();
    // Process dealer turn and resolve round outcome.
    void finishRound();

    // Check if a given text object was clicked.
    bool isTextClicked(const sf::Text& text, sf::RenderWindow& window);
};

#endif // GAME_H
