#include "MainMenu.h"

// Constructor - sets up the menu options and their positions
MainMenu::MainMenu(float width, float height) : selectedIndex(0) {
    // Load the font for the menu text
    if (!font.openFromFile("assets/ttfFont.ttf")) {
        std::cout << "Font not found!" << std::endl;
    }

    // Create "Play" option
    sf::Text playText(font, "Play", 70);
    playText.setFillColor(sf::Color::Yellow); // Highlighted by default
    playText.setPosition(sf::Vector2f(100.f, 200.f));

    // Create "Simulate" option
    sf::Text simulateText(font, "Simulate", 70);
    simulateText.setFillColor(sf::Color::White);
    simulateText.setPosition(sf::Vector2f(100.f, 300.f));

    // Create "Exit" option
    sf::Text exitText(font, "Exit", 70);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(sf::Vector2f(100.f, 400.f));

    // Add options to vector
    options.push_back(playText);
    options.push_back(simulateText);
    options.push_back(exitText);
}

// Destructor
MainMenu::~MainMenu() {
    // Nothing to clean up for now
}

// Draws the menu options to the window
void MainMenu::draw(sf::RenderWindow& window) {
    for (const auto& option : options) {
        window.draw(option);
    }
}

// Handles mouse hover to update highlighted option
void MainMenu::handleEvent(sf::Event event, sf::RenderWindow& window) {
    if (event.is<sf::Event::MouseMoved>()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        for (int i = 0; i < options.size(); i++) {
            if (options[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                // Highlight newly hovered option
                options[selectedIndex].setFillColor(sf::Color::White); // Reset old
                selectedIndex = i;
                options[selectedIndex].setFillColor(sf::Color::Yellow); // Highlight new
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
