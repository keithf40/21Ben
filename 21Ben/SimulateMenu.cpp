#include "SimulateMenu.h"

// Constructor - sets up the "Back" button
SimulateMenu::SimulateMenu(float width, float height) {
    // Load the font from file
    if (!font.openFromFile("assets/ttfFont.ttf")) {
        std::cout << "Font not found!" << std::endl;
    }

    // Create "Back" button
    sf::Text backButton(font, "Back", 50);
    backButton.setFillColor(sf::Color::White);
    backButton.setPosition(sf::Vector2f(10.f, height - 70.f));

    // Add it to the list of options
    options.push_back(backButton);

    // Set default selected index
    selectedIndex = 0;
}

// Draw the simulate menu (currently only the Back button)
void SimulateMenu::draw(sf::RenderWindow& window) {
    window.draw(options[0]);
}

// Handle mouse hover over the Back button
void SimulateMenu::handleEvent(sf::Event event, sf::RenderWindow& window) {
    if (event.is<sf::Event::MouseMoved>()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        for (int i = 0; i < options.size(); i++) {
            if (options[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                options[selectedIndex].setFillColor(sf::Color::White); // reset previous
                selectedIndex = i;
                options[selectedIndex].setFillColor(sf::Color::Yellow); // highlight new
            }
            else if (i != selectedIndex) {
                options[i].setFillColor(sf::Color::White); // reset unselected
            }
        }
    }
}

// Return the selected option (only "Back" in this case)
SimulateMenu::Option SimulateMenu::getSelectedOption() const {
    switch (selectedIndex) {
    case 0: return Option::BACK;
    default: return Option::NONE;
    }
}
