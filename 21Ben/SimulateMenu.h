#ifndef SIMULATE_MENU_H
#define SIMULATE_MENU_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

// Class representing the simulate menu screen
class SimulateMenu {
public:
    // Enum for selectable options in the simulate menu
    enum class Option { BACK, NONE };

    // Constructor
    SimulateMenu(float width, float height);

    // Draw the simulate menu options to the window
    void draw(sf::RenderWindow& window);

    // Handle mouse interaction events
    void handleEvent(sf::Event event, sf::RenderWindow& window);

    // Get the currently selected option
    Option getSelectedOption() const;

private:
    sf::Font font;                  // Font for the menu text
    std::vector<sf::Text> options;  // Text objects for each menu option
    int selectedIndex;              // Currently selected option index
};

#endif // SIMULATE_MENU_H
