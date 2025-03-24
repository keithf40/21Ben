#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

// Class representing the main menu screen
class MainMenu {
public:
    // Enum for menu options
    enum class Option { PLAY, SIMULATE, EXIT, NONE };

    // Constructor and destructor
    MainMenu(float width, float height);
    ~MainMenu();

    // Draws the menu options on the window
    void draw(sf::RenderWindow& window);

    // Handles mouse events for selection
    void handleEvent(sf::Event event, sf::RenderWindow& window);

    // Returns the currently selected menu option
    Option getSelectedOption() const;

    // Returns the bounding box of the selected option (for click detection)
    sf::FloatRect getSelectedOptionPos() const;

private:
    std::vector<sf::Text> options;  // Stores text elements for menu options
    sf::Font font;                  // Font used for all menu text
    int selectedIndex;              // Currently selected/hovered option index
};

#endif // MAIN_MENU_H
