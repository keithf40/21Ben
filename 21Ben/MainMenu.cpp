#include "MainMenu.h"
#include <iostream>

// Constructor - sets up the menu options and their positions
MainMenu::MainMenu(float width, float height) : selectedIndex(0) {
    // Load the font for the menu text
    if (!font.loadFromFile("assets/ttfFont.ttf")) {
        std::cerr << "Font not found!" << std::endl;
    }

    // Menu option labels and positions.
    // Updated labels vector to include Statistics
    const std::vector<std::string> labels = { "Play", "Game Settings", "Statistics", "Simulate", "Exit" };
    const float startY = 100.f;
    const float spacing = 100.f;

    for (std::size_t i = 0; i < labels.size(); ++i) {
        sf::Text option;
        option.setFont(font);
        option.setString(labels[i]);
        option.setCharacterSize(70);
        // Highlight the first option (Play) by default.
        option.setFillColor(i == 0 ? sf::Color::Yellow : sf::Color::White);
        option.setPosition(sf::Vector2f(50.f, startY + i * spacing));
        options.push_back(option);
    }
}

// Destructor
MainMenu::~MainMenu() {
    // Nothing to clean up as all data is managed by STL containers.
}

// Draws the menu options to the window
void MainMenu::draw(sf::RenderWindow& window) {
    for (const auto& option : options) {
        window.draw(option);
    }
}

// Handles mouse hover events to update the highlighted (selected) option
void MainMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        for (std::size_t i = 0; i < options.size(); ++i) {
            if (options[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                if (i != selectedIndex) {
                    // Reset the previously selected option and highlight the new one
                    options[selectedIndex].setFillColor(sf::Color::White);
                    selectedIndex = i;
                    options[selectedIndex].setFillColor(sf::Color::Yellow);
                }
            }
            else if (i != selectedIndex) {
                options[i].setFillColor(sf::Color::White);
            }
        }
    }
}

// Returns which menu option is currently selected based on selectedIndex
MainMenu::Option MainMenu::getSelectedOption() const {
    switch (selectedIndex) {
    case 0: return Option::PLAY;
    case 1: return Option::GAME_SETTINGS;
    case 2: return Option::STATISTICS;
    case 3: return Option::SIMULATE;
    case 4: return Option::EXIT;
    default: return Option::NONE;
    }
}

// Returns the global bounds (position and size) of the selected option for click detection
sf::FloatRect MainMenu::getSelectedOptionPos() const {
    return options[selectedIndex].getGlobalBounds();
}