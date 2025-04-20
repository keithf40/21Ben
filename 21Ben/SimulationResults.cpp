#include "SimulationResults.h"
#include <iostream>

SimulationResults::SimulationResults(float width, float height) {
    resultBox.setSize(sf::Vector2f(width * 0.8f, height * 0.8f));
    resultBox.setFillColor(sf::Color(30, 30, 30)); // Dark dark gray
    resultBox.setOutlineColor(sf::Color::Yellow);
    resultBox.setOutlineThickness(4.f);
    resultBox.setPosition(width * 0.1f, height * 0.1f);

    if (!font.loadFromFile("assets/ttfFont.ttf")) {
        std::cerr << "Font not found!" << std::endl;
    }

    // Back button setup (bottom left, same as PlayMenu)
    backButton.setFont(font);
    backButton.setString("Back");
    backButton.setCharacterSize(50);
    backButton.setFillColor(sf::Color::White);
    backButton.setPosition(10.f, height / 1.1f);
}

void SimulationResults::draw(sf::RenderWindow& window) {
    window.draw(resultBox);
    window.draw(backButton);
}

bool SimulationResults::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    bool hovering = backButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    backButton.setFillColor(hovering ? sf::Color::Yellow : sf::Color::White);

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (hovering) return true;
    }
    return false;
}
