#include "Game.h"

Game::Game(float width, float height) {
    if (!font.openFromFile("assets/ttfFont.ttf")) {
        std::cout << "Font not found!" << std::endl;
    }
    sf::Text skipButton(font, "Skip", 50);
    skipButton.setFillColor(sf::Color::White);
    skipButton.setPosition(sf::Vector2f(width - 80.f, 10.f));
    options.push_back(skipButton);
}

void Game::draw(sf::RenderWindow& window) {
    window.draw(options[0]);
}

void Game::handleEvent(sf::Event event, sf::RenderWindow& window) {
    if (event.is<sf::Event::MouseMoved>()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        for (int i = 0; i < options.size(); i++) {
            if (options[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                options[selectedIndex].setFillColor(sf::Color::Yellow);
                selectedIndex = i;
            }
            else {
                options[i].setFillColor(sf::Color::White);
            }
        }
    }
}

Game::Option Game::getSelectedOption() const {
    switch (selectedIndex) {
    case 0: return Option::SKIP;
    default: return Option::NONE;
    }
}

std::vector<std::string> Game::getGameSettings() const {
    return gameSettings;
}


