#pragma once
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

class Game {
public:
    enum class Option { SKIP, NONE };

    Game(float width, float height);

    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    Option getSelectedOption() const;
    std::vector<std::string> getGameSettings() const;

    // gameSettings by index => 
    // 0: Allow Assistance, 1: Min Bet, 2: Starting $, 3: Select Playstyle

private:
    sf::Font font;
    std::vector<sf::Text> options;
    std::vector<std::string> gameSettings = { "NO", "15", "200", "Standard" };
    std::vector<sf::RectangleShape> cards;
    int selectedIndex = 0;
};

#endif // GAME_H
