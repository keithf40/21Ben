#include "PlayMenu.h"
#include <iostream>

// Constructor - initializes the "Back" and "Play" buttons
PlayMenu::PlayMenu(float width, float height) : selectedIndex(0) {
    // Load the font used for menu text
    if (!font.loadFromFile("assets/ttfFont.ttf")) {
        std::cerr << "Font not found!" << std::endl;
    }

    // Create "Back" button
    sf::Text backButton;
    backButton.setFont(font);
    backButton.setString("Back");
    backButton.setCharacterSize(50);
    backButton.setFillColor(sf::Color::White);
    backButton.setPosition(sf::Vector2f(10.f, height - 70.f));

    // Create "Play" button
    sf::Text playButton;
    playButton.setFont(font);
    playButton.setString("Play");
    playButton.setCharacterSize(50);
    playButton.setFillColor(sf::Color::White);
    playButton.setPosition(sf::Vector2f(width / 2.1f, height / 2.5f));

    // Add buttons to list
    options.push_back(backButton);
    options.push_back(playButton);
}

// Draw all the buttons in the play menu
void PlayMenu::draw(sf::RenderWindow& window) {
    for (const auto& option : options) {
        window.draw(option);
    }
}

// Handle mouse movement to update highlighted button
void PlayMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        for (std::size_t i = 0; i < options.size(); ++i) {
            if (options[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                if (i != selectedIndex) {
                    options[selectedIndex].setFillColor(sf::Color::White); // reset old
                    selectedIndex = i;
                    options[selectedIndex].setFillColor(sf::Color::Yellow); // highlight new
                }
            }
            else if (i != selectedIndex) {
                options[i].setFillColor(sf::Color::White); // reset unhovered
            }
        }
    }
}

// Get the currently selected menu option
PlayMenu::Option PlayMenu::getSelectedOption() const {
    switch (selectedIndex) {
    case 0: return Option::BACK;
    case 1: return Option::START;
    default: return Option::NONE;
    }
}
