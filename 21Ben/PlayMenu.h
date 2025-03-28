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
    std::vector<int> getGameSettings() const { return gameSettings; }

private:
    std::vector<sf::Text> options;
    // gameSettings[0]: checkbox state, gameSettings[1]: minimum bet (15-200),
    // gameSettings[2]: starting money (100-5000), gameSettings[3]: amount of decks, gameSettings[4]: player position (1-5)
    std::vector<int> gameSettings = { 0, 15, 100, 1, 1 };
    std::vector<std::string> strategies = { "Standard", "HiLo", "HiOpt1", "HiOpt2", "Mentor", "Omega2",
                                              "Revere Point Count", "Revere 14 Count", "Revere RAPC",
                                              "Wong Halves", "Zen Count", "KO" };
    std::vector<std::string> amountOfDecks = { "Single Deck", "Multi-Deck (6)", "Multi-Deck (8)" };
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

    // Dropdown menu for strategy selection (affected by assistance)
    sf::RectangleShape dropdownBox;
    sf::Text dropdownText;
    bool dropdownOpen;
    std::vector<sf::Text> dropdownOptions;
    std::size_t dropdownSelectedIndex;

    // Dropdown menu for amount of decks (always enabled)
    sf::RectangleShape deckDropdownBox;
    sf::Text deckDropdownText;
    bool deckDropdownOpen;
    std::vector<sf::Text> deckDropdownOptions;
    std::size_t deckDropdownSelectedIndex;

    // New slider for Player Position (range 1-5)
    sf::RectangleShape slider3Track;
    sf::RectangleShape slider3Knob;
    sf::Text slider3Label;
    bool isDraggingSlider3;
};
