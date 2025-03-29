#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class SimulateMenu {
public:
    enum class Option { BACK, START, NONE };

    SimulateMenu(float width, float height);
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    Option getSelectedOption() const;
    sf::FloatRect getSelectedOptionPos() const;
    std::string getSelectedStrategy() const { return selectedStrategy; }
    std::string getSelectedStrategy2() const { return selectedStrategy2; }
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
    // New dropdown for "Select Playstyle" (always enabled) updates selectedStrategy
    std::string selectedStrategy = "Standard";
    // Existing dropdown for compare playstyle (affected by assistance) updates selectedStrategy2
    std::string selectedStrategy2 = "Standard";
    sf::Font font;
    std::size_t selectedIndex;

    // Title text
    sf::Text titleText;

    // Checkbox elements (for "Compare Playstyle to")
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

    // Dropdown for "Compare Playstyle to" (affected by assistance)
    sf::RectangleShape dropdownBox;
    sf::Text dropdownText;
    bool dropdownOpen;
    std::vector<sf::Text> dropdownOptions;
    std::size_t dropdownSelectedIndex;

    // Dropdown for amount of decks (always enabled)
    sf::RectangleShape deckDropdownBox;
    sf::Text deckDropdownText;
    bool deckDropdownOpen;
    std::vector<sf::Text> deckDropdownOptions;
    std::size_t deckDropdownSelectedIndex;

    // Slider for Player Position (range 1-5)
    sf::RectangleShape slider3Track;
    sf::RectangleShape slider3Knob;
    sf::Text slider3Label;
    bool isDraggingSlider3;

    // New Dropdown for "Select Playstyle" (always enabled)
    sf::RectangleShape playstyleDropdownBox;
    sf::Text playstyleDropdownText;
    bool playstyleDropdownOpen;
    std::vector<sf::Text> playstyleDropdownOptions;
    std::size_t playstyleDropdownSelectedIndex;

    // Label for "Select Playstyle"
    sf::Text playstyleDropdownLabel;
};
