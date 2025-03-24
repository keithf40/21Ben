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

private:
    std::vector<sf::Text> options;
    sf::Font font;
    std::size_t selectedIndex;
};
