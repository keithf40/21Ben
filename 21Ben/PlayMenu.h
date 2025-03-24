#ifndef PLAY_MENU_H
#define PLAY_MENU_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

// Class representing the "Play" menu screen
class PlayMenu {
public:
    // Enum for selectable options in the play menu
    enum class Option { BACK, START, NONE };

    // Constructor
    PlayMenu(float width, float height);

    // Renders the menu options
    void draw(sf::RenderWindow& window);

    // Handles mouse movement (hover detection)
    void handleEvent(sf::Event event, sf::RenderWindow& window);

    // Returns the currently selected option
    Option getSelectedOption() const;

private:
    sf::Font font;                  // Font used for menu text
    std::vector<sf::Text> options;  // Menu options (Back and Play)
    int selectedIndex;              // Index of the currently hovered option
};

#endif // PLAY_MENU_H
