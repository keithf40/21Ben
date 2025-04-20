#pragma once
#include <SFML/Graphics.hpp>

class SimulationResults {
public:
    SimulationResults(float width, float height);
    void draw(sf::RenderWindow& window);
    bool handleEvent(const sf::Event& event, sf::RenderWindow& window); // returns true if back clicked

private:
    sf::RectangleShape resultBox;
    sf::Font font;
    sf::Text backButton;
};
