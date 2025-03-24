#include "PlayMenu.h"

// Constructor - initializes the "Back" and "Play" buttons
PlayMenu::PlayMenu(float width, float height) {
    // Load the font used for menu text
    if (!font.openFromFile("assets/ttfFont.ttf")) {
        std::cout << "Font not found!" << std::endl;
    }

    // Create "Back" button
    sf::Text backButton(font, "Back", 50);
    backButton.setFillColor(sf::Color::White);
    backButton.setPosition(sf::Vector2f(10.f, height - 70.f));

    // Create "Play" button
    sf::Text playButton(font, "Play", 50);
    playButton.setFillColor(sf::Color::White);
    playButton.setPosition(sf::Vector2f(width / 2.1f, height - (height / 2.5f)));

    // Add both buttons to the list of options
    options.push_back(backButton);
    options.push_back(playButton);

    // Set default selected index
    selectedIndex = 0;
}

// Draw all the buttons in the play menu
void PlayMenu::draw(sf::RenderWindow& window) {
    for (int i = 0; i < options.size(); i++) {
        window.draw(options[i]);
    }
}

// Handle mouse movement to update highlighted button
void PlayMenu::handleEvent(sf::Event event, sf::RenderWindow& window) {
    if (event.is<sf::Event::MouseMoved>()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        for (int i = 0; i < options.size(); i++) {
            if (options[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                // Reset previously selected
                options[selectedIndex].setFillColor(sf::Color::White);
                // Highlight current hover
                selectedIndex = i;
                options[selectedIndex].setFillColor(sf::Color::Yellow);
            }
            else if (i != selectedIndex) {
                options[i].setFillColor(sf::Color::White);
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
