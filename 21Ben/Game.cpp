#include "Game.h"
#include <iostream>

Game::Game(float width, float height) {
    if (!font.loadFromFile("assets/ttfFont.ttf")) {
        std::cerr << "Font not found!" << std::endl;
    }

    // Create "Skip" button
    sf::Text skipButton;
    skipButton.setFont(font);
    skipButton.setString("Skip");
    skipButton.setCharacterSize(50);
    skipButton.setFillColor(sf::Color::White);
    skipButton.setPosition(sf::Vector2f(width - 120.f, 10.f)); // better alignment
    options.push_back(skipButton);

    selectedIndex = 0;
}

void Game::draw(sf::RenderWindow& window) {
    for (const auto& option : options) {
        window.draw(option);
    }
}

void Game::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        for (std::size_t i = 0; i < options.size(); ++i) {
            if (options[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                if (i != selectedIndex) {
                    options[selectedIndex].setFillColor(sf::Color::White); // reset previous
                    selectedIndex = i;
                    options[selectedIndex].setFillColor(sf::Color::Yellow); // highlight new
                }
            }
            else if (i != selectedIndex) {
                options[i].setFillColor(sf::Color::White);
            }
        }
    }
}

Game::Option Game::getSelectedOption() const {
    if (selectedIndex == 0) {
        return Option::SKIP;
    }
    return Option::NONE;
}

std::vector<std::string> Game::getGameSettings() const {
    return gameSettings;
}
