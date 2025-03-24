#include "MainMenu.h"
#include <iostream>

// Constructor - sets up the menu options and their positions
MainMenu::MainMenu(float width, float height) : selectedIndex(0) {
    // Load the font for the menu text
    if (!font.loadFromFile("assets/ttfFont.ttf")) {
        std::cerr << "Font not found!" << std::endl;
    }

    // Menu option labels and positions
    const std::vector<std::string> labels = { "Play", "Simulate", "Exit" };
    const float startY = 200.f;
    const float spacing = 100.f;

    for (std::size_t i = 0; i < labels.size(); ++i) {
        sf::Text option;
        option.setFont(font);
        option.setString(labels[i]);
        option.setCharacterSize(70);
        option.setFillColor(i == 0 ? sf::Color::Yellow : sf::Color::White); // Highlight first by default
        option.setPosition(sf::Vector2f(100.f, startY + i * spacing));
        options.push_back(option);
    }
}

// Destructor
MainMenu::~MainMenu() {
    // No dynamic memory to clean up
}

// Draws the menu options to the window
void MainMenu::draw(sf::RenderWindow& window) {
    for (const auto& option : options) {
        window.draw(option);
    }
}

// Handles mouse hover to update highlighted option
void MainMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        for (std::size_t i = 0; i < options.size(); ++i) {
            if (options[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                if (i != selectedIndex) {
                    options[selectedIndex].setFillColor(sf::Color::White); // Reset old
                    selectedIndex = i;
                    options[selectedIndex].setFillColor(sf::Color::Yellow); // Highlight new
                }
            }
            else if (i != selectedIndex) {
                options[i].setFillColor(sf::Color::White); // Reset others
            }
        }
    }
}

// Returns which menu option is currently selected
MainMenu::Option MainMenu::getSelectedOption() const {
    switch (selectedIndex) {
    case 0: return Option::PLAY;
    case 1: return Option::SIMULATE;
    case 2: return Option::EXIT;
    default: return Option::NONE;
    }
}

// Returns the global bounds (position + size) of the selected option
sf::FloatRect MainMenu::getSelectedOptionPos() const {
    return options[selectedIndex].getGlobalBounds();
}
