#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class PlayMenu {
public:
    enum class Option { BACK, START, NONE };

    PlayMenu(float width, float height);
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    Option getSelectedOption() const;
    sf::FloatRect getSelectedOptionPos() const;

private:
    std::vector<sf::Text> options;
    std::vector<int> gameSettings = { 0 };
    sf::Font font;
    std::size_t selectedIndex;

    // New checkbox elements
    sf::RectangleShape checkbox;
    bool isChecked;
    sf::Text checkboxLabel; // Optional label for the checkbox
};
