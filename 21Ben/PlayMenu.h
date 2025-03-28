#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class PlayMenu {
public:
    enum class Option { BACK, START, NONE };

    PlayMenu(float width, float height);
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    Option getSelectedOption() const;
    sf::FloatRect getSelectedOptionPos() const;
    std::string getSelectedStrategy() const { return selectedStrategy; }

private:
    std::vector<sf::Text> options;
    // gameSettings[0]: checkbox state, gameSettings[1]: minimum bet (15-200), gameSettings[2]: starting money (100-5000)
    std::vector<int> gameSettings = { 0, 15, 100 };
    std::vector<std::string> strategies = { "Standard", "HiLo", "HiOpt1", "HiOpt2", "Mentor", "Omega2",
                                              "Revere Point Count", "Revere 14 Count", "Revere RAPC",
                                              "Wong Halves", "Zen Count", "KO" };
    std::string selectedStrategy = "Standard";
    sf::Font font;
    std::size_t selectedIndex;

    // Title
    sf::Text titleText;

    // Checkbox elements
    sf::RectangleShape checkbox;
    bool isChecked;
    sf::Text checkboxLabel;

    // Slider for minimum bet
    sf::RectangleShape sliderTrack;
    sf::RectangleShape sliderKnob;
    sf::Text sliderLabel;
    bool isDraggingSlider;

    // Slider for starting money
    sf::RectangleShape slider2Track;
    sf::RectangleShape slider2Knob;
    sf::Text slider2Label;
    bool isDraggingSlider2;

    // Dropdown menu for strategy selection
    sf::RectangleShape dropdownBox;
    sf::Text dropdownText;
    bool dropdownOpen;
    std::vector<sf::Text> dropdownOptions;
    std::size_t dropdownSelectedIndex;
};
