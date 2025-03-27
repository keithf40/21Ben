#include "PlayMenu.h"
#include <iostream>

// Constructor - initializes the "Back" and "Play" buttons and the checkbox
PlayMenu::PlayMenu(float width, float height)
    : selectedIndex(0), isChecked(false)
{
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

    // Initialize the checkbox (20x20 pixels)
    checkbox.setSize(sf::Vector2f(20.f, 20.f));
    // Position it below the "Play" button (adjust as needed)
    checkbox.setPosition(sf::Vector2f(width / 2.1f, height / 2.5f + 70.f));
    checkbox.setFillColor(sf::Color::White);

    // Initialize the checkbox label (optional)
    checkboxLabel.setFont(font);
    checkboxLabel.setString("Enable Assistance");
    checkboxLabel.setCharacterSize(30);
    checkboxLabel.setFillColor(sf::Color::White);
    // Position label next to the checkbox
    checkboxLabel.setPosition(checkbox.getPosition().x + 30.f, checkbox.getPosition().y - 5.f);
}

// Draw all the buttons and the checkbox in the play menu
void PlayMenu::draw(sf::RenderWindow& window) {
    for (const auto& option : options) {
        window.draw(option);
    }
    window.draw(checkbox);
    window.draw(checkboxLabel);
}

// Handle mouse events to update highlighted button and checkbox state
void PlayMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    // Handle mouse movement for button highlighting
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

    // Handle mouse button press for the checkbox
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (checkbox.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            // Toggle checkbox state
            isChecked = !isChecked;
            gameSettings[0] = isChecked ? 1 : 0;
            // Change the checkbox color to indicate its state
            checkbox.setFillColor(isChecked ? sf::Color::Green : sf::Color::White);
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

sf::FloatRect PlayMenu::getSelectedOptionPos() const {
    return options[selectedIndex].getGlobalBounds();
}
