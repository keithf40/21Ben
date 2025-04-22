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
    // gameSettings[0]: checkbox state, gameSettings[1]: minimum bet (5-200),
    // gameSettings[2]: starting money (100-10000), gameSettings[3]: deck count (1-8), gameSettings[4]: simulations (100-1000)
    std::vector<int> gameSettings = { 0, 5, 100, 1, 100 };
    std::vector<std::string> strategies = { "None", "HiLo", "HiOpt1", "HiOpt2", "Mentor", "Omega2",
                                              "Revere Point Count", "Revere 14 Count", "Revere RAPC",
                                              "Wong Halves", "Zen Count", "KO" };
    // Removed amountOfDecks and deck dropdown members
    // std::vector<std::string> amountOfDecks = { "Single Deck", "Multi-Deck (6)", "Multi-Deck (8)" };

    // New dropdown for "Select Playstyle" (always enabled) updates selectedStrategy
    std::string selectedStrategy = "None";
    // Existing dropdown for compare playstyle (affected by assistance) updates selectedStrategy2
    std::string selectedStrategy2 = "None";
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

    // New Slider for deck count (range 1-8) replacing the deck dropdown
    sf::RectangleShape deckSliderTrack;
    sf::RectangleShape deckSliderKnob;
    sf::Text deckSliderLabel;
    bool isDraggingDeckSlider;

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
