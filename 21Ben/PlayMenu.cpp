#include "PlayMenu.h"
#include <iostream>
#include <cmath>  // For std::round

// Constructor - initializes buttons, checkbox, sliders, and both dropdowns, plus the new Player Position slider
PlayMenu::PlayMenu(float width, float height)
    : selectedIndex(0), isChecked(false), isDraggingSlider(false), isDraggingSlider2(false),
    dropdownOpen(false), dropdownSelectedIndex(0), deckDropdownOpen(false), deckDropdownSelectedIndex(0),
    isDraggingSlider3(false)
{
    // Load the font used for menu text
    if (!font.loadFromFile("assets/ttfFont.ttf")) {
        std::cerr << "Font not found!" << std::endl;
    }

    // Title
    titleText.setFont(font);
    titleText.setString("Game Settings");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(sf::Vector2f(width / 2.50f, height / 3.25f));

    // Create "Back" button
    sf::Text backButton;
    backButton.setFont(font);
    backButton.setString("Back");
    backButton.setCharacterSize(50);
    backButton.setFillColor(sf::Color::White);
    backButton.setPosition(sf::Vector2f(10.f, height / 1.1f));

    // Create "Play" button
    sf::Text playButton;
    playButton.setFont(font);
    playButton.setString("Play");
    playButton.setCharacterSize(35);
    playButton.setFillColor(sf::Color::White);
    playButton.setPosition(sf::Vector2f(width / 2.1f, height / 1.5f));

    // Add buttons to list
    options.push_back(backButton);
    options.push_back(playButton);

    // Initialize the checkbox (20x20 pixels)
    checkbox.setSize(sf::Vector2f(20.f, 20.f));
    checkbox.setPosition(sf::Vector2f(width / 3.9f, height / 2.5f));
    checkbox.setFillColor(sf::Color::White);

    // Initialize the checkbox label
    checkboxLabel.setFont(font);
    checkboxLabel.setString("Enable Assistance");
    checkboxLabel.setCharacterSize(30);
    checkboxLabel.setFillColor(sf::Color::White);
    checkboxLabel.setPosition(checkbox.getPosition().x + 30.f, checkbox.getPosition().y - 5.f);

    // --------------------------
    // Initialize the strategy dropdown (affected by assistance)
    dropdownBox.setSize(sf::Vector2f(150.f, 30.f));
    dropdownBox.setPosition(checkbox.getPosition().x, checkbox.getPosition().y + checkbox.getSize().y + 10.f);
    dropdownBox.setFillColor(isChecked ? sf::Color::White : sf::Color(128, 128, 128));

    dropdownText.setFont(font);
    dropdownText.setString(selectedStrategy);
    dropdownText.setCharacterSize(20);
    dropdownText.setFillColor(sf::Color::Black);
    dropdownText.setPosition(dropdownBox.getPosition().x + 5.f, dropdownBox.getPosition().y + 5.f);

    for (std::size_t i = 0; i < strategies.size(); ++i) {
        sf::Text option;
        option.setFont(font);
        option.setString(strategies[i]);
        option.setCharacterSize(20);
        option.setFillColor(sf::Color::White);
        option.setPosition(dropdownBox.getPosition().x,
            dropdownBox.getPosition().y + dropdownBox.getSize().y * (i + 1));
        dropdownOptions.push_back(option);
    }

    // --------------------------
    // Initialize the deck dropdown (always enabled)
    deckDropdownBox.setSize(sf::Vector2f(150.f, 30.f));
    deckDropdownBox.setPosition(dropdownBox.getPosition().x, dropdownBox.getPosition().y + dropdownBox.getSize().y + 20.f);
    deckDropdownBox.setFillColor(sf::Color::White);

    deckDropdownText.setFont(font);
    deckDropdownText.setString(amountOfDecks[0]); // default: "Single Deck"
    deckDropdownText.setCharacterSize(20);
    deckDropdownText.setFillColor(sf::Color::Black);
    deckDropdownText.setPosition(deckDropdownBox.getPosition().x + 5.f, deckDropdownBox.getPosition().y + 5.f);

    for (std::size_t i = 0; i < amountOfDecks.size(); ++i) {
        sf::Text option;
        option.setFont(font);
        option.setString(amountOfDecks[i]);
        option.setCharacterSize(20);
        option.setFillColor(sf::Color::White);
        option.setPosition(deckDropdownBox.getPosition().x,
            deckDropdownBox.getPosition().y + deckDropdownBox.getSize().y * (i + 1));
        deckDropdownOptions.push_back(option);
    }

    // --------------------------
    // Position sliders to the right of the assistance area.
    float sliderX = checkboxLabel.getPosition().x + checkboxLabel.getLocalBounds().width + 50.f;

    // Slider for minimum bet:
    sliderTrack.setSize(sf::Vector2f(250.f, 5.f));
    sliderTrack.setFillColor(sf::Color::White);
    sliderTrack.setPosition(sf::Vector2f(sliderX, checkbox.getPosition().y + 40.f));

    sliderKnob.setSize(sf::Vector2f(15.f, 25.f));
    sliderKnob.setFillColor(sf::Color::Yellow);
    {
        float ratio = float(gameSettings[1] - 15) / float(200 - 15);
        float knobX = sliderTrack.getPosition().x + ratio * sliderTrack.getSize().x;
        sliderKnob.setPosition(knobX - sliderKnob.getSize().x / 2,
            sliderTrack.getPosition().y - (sliderKnob.getSize().y - sliderTrack.getSize().y) / 2);
    }

    sliderLabel.setFont(font);
    sliderLabel.setString("Min Bet: " + std::to_string(gameSettings[1]));
    sliderLabel.setCharacterSize(25);
    sliderLabel.setFillColor(sf::Color::White);
    sliderLabel.setPosition(sliderTrack.getPosition().x, sliderTrack.getPosition().y - 40.f);

    // Slider for starting money:
    slider2Track.setSize(sf::Vector2f(250.f, 5.f));
    slider2Track.setFillColor(sf::Color::White);
    slider2Track.setPosition(sf::Vector2f(sliderX, sliderTrack.getPosition().y + 60.f));

    slider2Knob.setSize(sf::Vector2f(15.f, 25.f));
    slider2Knob.setFillColor(sf::Color::Yellow);
    {
        float ratio2 = float(gameSettings[2] - 100) / float(5000 - 100);
        float knob2X = slider2Track.getPosition().x + ratio2 * slider2Track.getSize().x;
        slider2Knob.setPosition(knob2X - slider2Knob.getSize().x / 2,
            slider2Track.getPosition().y - (slider2Knob.getSize().y - slider2Track.getSize().y) / 2);
    }

    slider2Label.setFont(font);
    slider2Label.setString("Starting Money: " + std::to_string(gameSettings[2]));
    slider2Label.setCharacterSize(25);
    slider2Label.setFillColor(sf::Color::White);
    slider2Label.setPosition(slider2Track.getPosition().x, slider2Track.getPosition().y - 40.f);

    // --------------------------
    // New Slider for Player Position (range 1-5)
    slider3Track.setSize(sf::Vector2f(250.f, 5.f));
    slider3Track.setFillColor(sf::Color::White);
    // Position it below the deck dropdown
    slider3Track.setPosition(sf::Vector2f(width / 2.5f, deckDropdownBox.getPosition().y + deckDropdownBox.getSize().y + 50.f));

    slider3Knob.setSize(sf::Vector2f(15.f, 25.f));
    slider3Knob.setFillColor(sf::Color::Yellow);
    {
        float ratio = float(gameSettings[4] - 1) / float(5 - 1);
        float knobX = slider3Track.getPosition().x + ratio * slider3Track.getSize().x;
        slider3Knob.setPosition(knobX - slider3Knob.getSize().x / 2,
            slider3Track.getPosition().y - (slider3Knob.getSize().y - slider3Track.getSize().y) / 2);
    }

    slider3Label.setFont(font);
    slider3Label.setString("Player Position: " + std::to_string(gameSettings[4]));
    slider3Label.setCharacterSize(25);
    slider3Label.setFillColor(sf::Color::White);
    slider3Label.setPosition(slider3Track.getPosition().x, slider3Track.getPosition().y - 40.f);
}

// Draw all UI elements
void PlayMenu::draw(sf::RenderWindow& window) {
    // Draw buttons, title, checkbox and sliders (min bet & starting money)
    for (const auto& option : options) {
        window.draw(option);
    }
    window.draw(titleText);
    window.draw(checkbox);
    window.draw(checkboxLabel);
    window.draw(sliderTrack);
    window.draw(sliderKnob);
    window.draw(sliderLabel);
    window.draw(slider2Track);
    window.draw(slider2Knob);
    window.draw(slider2Label);

    // Draw strategy dropdown and its options (if open and assistance enabled)
    window.draw(dropdownBox);
    window.draw(dropdownText);
    if (dropdownOpen && isChecked) {
        sf::RectangleShape dropdownBackground;
        float bgX = dropdownBox.getPosition().x;
        float bgY = dropdownBox.getPosition().y + dropdownBox.getSize().y;
        float bgWidth = dropdownBox.getSize().x;
        float bgHeight = dropdownBox.getSize().y * static_cast<float>(dropdownOptions.size());
        dropdownBackground.setSize(sf::Vector2f(bgWidth, bgHeight));
        dropdownBackground.setPosition(bgX, bgY);
        dropdownBackground.setFillColor(sf::Color::White);
        window.draw(dropdownBackground);
        for (const auto& option : dropdownOptions) {
            window.draw(option);
        }
    }

    // Reposition the deck dropdown so it does not overlap the strategy dropdown when it is open.
    if (dropdownOpen && isChecked) {
        float newDeckY = dropdownBox.getPosition().y + dropdownBox.getSize().y * (dropdownOptions.size() + 1) + 20.f;
        deckDropdownBox.setPosition(deckDropdownBox.getPosition().x, newDeckY);
        deckDropdownText.setPosition(deckDropdownBox.getPosition().x + 5.f, deckDropdownBox.getPosition().y + 5.f);
        for (std::size_t i = 0; i < deckDropdownOptions.size(); ++i) {
            deckDropdownOptions[i].setPosition(
                deckDropdownBox.getPosition().x,
                deckDropdownBox.getPosition().y + deckDropdownBox.getSize().y * (i + 1)
            );
        }
    }
    else {
        // Default position when strategy dropdown is closed
        deckDropdownBox.setPosition(dropdownBox.getPosition().x, dropdownBox.getPosition().y + dropdownBox.getSize().y + 20.f);
        deckDropdownText.setPosition(deckDropdownBox.getPosition().x + 5.f, deckDropdownBox.getPosition().y + 5.f);
        for (std::size_t i = 0; i < deckDropdownOptions.size(); ++i) {
            deckDropdownOptions[i].setPosition(
                deckDropdownBox.getPosition().x,
                deckDropdownBox.getPosition().y + deckDropdownBox.getSize().y * (i + 1)
            );
        }
    }

    // Draw deck dropdown and its options (always enabled)
    window.draw(deckDropdownBox);
    window.draw(deckDropdownText);
    if (deckDropdownOpen) {
        sf::RectangleShape deckDropdownBackground;
        float bgX = deckDropdownBox.getPosition().x;
        float bgY = deckDropdownBox.getPosition().y + deckDropdownBox.getSize().y;
        float bgWidth = deckDropdownBox.getSize().x;
        float bgHeight = deckDropdownBox.getSize().y * static_cast<float>(deckDropdownOptions.size());
        deckDropdownBackground.setSize(sf::Vector2f(bgWidth, bgHeight));
        deckDropdownBackground.setPosition(bgX, bgY);
        deckDropdownBackground.setFillColor(sf::Color::White);
        window.draw(deckDropdownBackground);
        for (const auto& option : deckDropdownOptions) {
            window.draw(option);
        }
    }

    // Draw the new Player Position slider
    window.draw(slider3Track);
    window.draw(slider3Knob);
    window.draw(slider3Label);
}

// Handle events: updates for buttons, checkbox, sliders, and both dropdowns plus the new slider
void PlayMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    // Update strategy dropdown appearance based on assistance; deck dropdown remains white.
    if (!isChecked)
        dropdownBox.setFillColor(sf::Color(128, 128, 128));
    else
        dropdownBox.setFillColor(sf::Color::White);
    deckDropdownBox.setFillColor(sf::Color::White);

    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Update slider for minimum bet if dragged
        if (isDraggingSlider) {
            float newX = static_cast<float>(mousePos.x);
            float leftBound = sliderTrack.getPosition().x;
            float rightBound = leftBound + sliderTrack.getSize().x;
            newX = (newX < leftBound) ? leftBound : (newX > rightBound ? rightBound : newX);
            sliderKnob.setPosition(newX - sliderKnob.getSize().x / 2, sliderKnob.getPosition().y);
            float ratio = (newX - leftBound) / sliderTrack.getSize().x;
            double raw = 15 + ratio * (200 - 15);
            int quantized = 15 + static_cast<int>(std::round((raw - 15) / 5.0)) * 5;
            if (quantized > 200) quantized = 200;
            gameSettings[1] = quantized;
            sliderLabel.setString("Min Bet: " + std::to_string(gameSettings[1]));
        }

        // Update slider for starting money if dragged
        if (isDraggingSlider2) {
            float newX = static_cast<float>(mousePos.x);
            float leftBound = slider2Track.getPosition().x;
            float rightBound = leftBound + slider2Track.getSize().x;
            newX = (newX < leftBound) ? leftBound : (newX > rightBound ? rightBound : newX);
            slider2Knob.setPosition(newX - slider2Knob.getSize().x / 2, slider2Knob.getPosition().y);
            float ratio2 = (newX - leftBound) / slider2Track.getSize().x;
            double raw2 = 100 + ratio2 * (5000 - 100);
            int quantized2 = 100 + static_cast<int>(std::round((raw2 - 100) / 50.0)) * 50;
            if (quantized2 > 5000) quantized2 = 5000;
            gameSettings[2] = quantized2;
            slider2Label.setString("Starting Money: " + std::to_string(gameSettings[2]));
        }

        // Update new slider for Player Position if dragged
        if (isDraggingSlider3) {
            float newX = static_cast<float>(mousePos.x);
            float leftBound = slider3Track.getPosition().x;
            float rightBound = leftBound + slider3Track.getSize().x;
            if (newX < leftBound) newX = leftBound;
            if (newX > rightBound) newX = rightBound;
            slider3Knob.setPosition(newX - slider3Knob.getSize().x / 2, slider3Knob.getPosition().y);
            float ratio = (newX - leftBound) / slider3Track.getSize().x;
            double raw = 1 + ratio * (5 - 1);
            int quantized = 1 + static_cast<int>(std::round(raw - 1));
            if (quantized < 1) quantized = 1;
            if (quantized > 5) quantized = 5;
            gameSettings[4] = quantized;
            slider3Label.setString("Player Position: " + std::to_string(gameSettings[4]));
        }

        // Highlight Back/Play options
        for (std::size_t i = 0; i < options.size(); ++i) {
            if (options[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                options[i].setFillColor(sf::Color::Yellow);
                selectedIndex = i;
            }
            else {
                options[i].setFillColor(sf::Color::White);
            }
        }

        // Highlight strategy dropdown options if open and assistance enabled
        if (dropdownOpen && isChecked) {
            for (auto& option : dropdownOptions) {
                option.setFillColor(option.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))
                    ? sf::Color::Yellow : sf::Color::Black);
            }
        }

        // Highlight deck dropdown options if open (always active)
        if (deckDropdownOpen) {
            for (auto& option : deckDropdownOptions) {
                option.setFillColor(option.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))
                    ? sf::Color::Yellow : sf::Color::Black);
            }
        }
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Checkbox toggle
        if (checkbox.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            isChecked = !isChecked;
            gameSettings[0] = isChecked ? 1 : 0;
            checkbox.setFillColor(isChecked ? sf::Color::Green : sf::Color::White);
            if (!isChecked) {
                dropdownOpen = false;
            }
        }

        // Slider interactions for min bet
        if (sliderKnob.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)) ||
            sliderTrack.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            isDraggingSlider = true;
            float newX = static_cast<float>(mousePos.x);
            float leftBound = sliderTrack.getPosition().x;
            float rightBound = leftBound + sliderTrack.getSize().x;
            newX = (newX < leftBound) ? leftBound : (newX > rightBound ? rightBound : newX);
            sliderKnob.setPosition(newX - sliderKnob.getSize().x / 2, sliderKnob.getPosition().y);
            float ratio = (newX - leftBound) / sliderTrack.getSize().x;
            double raw = 15 + ratio * (200 - 15);
            int quantized = 15 + static_cast<int>(std::round((raw - 15) / 5.0)) * 5;
            if (quantized > 200) quantized = 200;
            gameSettings[1] = quantized;
            sliderLabel.setString("Min Bet: " + std::to_string(gameSettings[1]));
        }

        // Slider interactions for starting money
        if (slider2Knob.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)) ||
            slider2Track.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            isDraggingSlider2 = true;
            float newX = static_cast<float>(mousePos.x);
            float leftBound = slider2Track.getPosition().x;
            float rightBound = leftBound + slider2Track.getSize().x;
            newX = (newX < leftBound) ? leftBound : (newX > rightBound ? rightBound : newX);
            slider2Knob.setPosition(newX - slider2Knob.getSize().x / 2, slider2Knob.getPosition().y);
            float ratio2 = (newX - leftBound) / slider2Track.getSize().x;
            double raw2 = 100 + ratio2 * (5000 - 100);
            int quantized2 = 100 + static_cast<int>(std::round((raw2 - 100) / 50.0)) * 50;
            if (quantized2 > 5000) quantized2 = 5000;
            gameSettings[2] = quantized2;
            slider2Label.setString("Starting Money: " + std::to_string(gameSettings[2]));
        }

        // New slider interactions for Player Position
        if (slider3Knob.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)) ||
            slider3Track.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            isDraggingSlider3 = true;
            float newX = static_cast<float>(mousePos.x);
            float leftBound = slider3Track.getPosition().x;
            float rightBound = leftBound + slider3Track.getSize().x;
            if (newX < leftBound) newX = leftBound;
            if (newX > rightBound) newX = rightBound;
            slider3Knob.setPosition(newX - slider3Knob.getSize().x / 2, slider3Knob.getPosition().y);
            float ratio = (newX - leftBound) / slider3Track.getSize().x;
            double raw = 1 + ratio * (5 - 1);
            int quantized = 1 + static_cast<int>(std::round(raw - 1));
            if (quantized < 1) quantized = 1;
            if (quantized > 5) quantized = 5;
            gameSettings[4] = quantized;
            slider3Label.setString("Player Position: " + std::to_string(gameSettings[4]));
        }

        // Strategy dropdown interaction (only if assistance is enabled)
        if (isChecked) {
            if (dropdownBox.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                dropdownOpen = !dropdownOpen;
                if (dropdownOpen)
                    deckDropdownOpen = false; // close deck dropdown when opening strategy dropdown
            }
            if (dropdownOpen) {
                for (std::size_t i = 0; i < dropdownOptions.size(); ++i) {
                    if (dropdownOptions[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        dropdownSelectedIndex = i;
                        selectedStrategy = strategies[i];
                        dropdownText.setString(selectedStrategy);
                        dropdownOpen = false;
                        break;
                    }
                }
            }
        }

        // Deck dropdown interaction (always active)
        if (deckDropdownBox.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            deckDropdownOpen = !deckDropdownOpen;
            if (deckDropdownOpen)
                dropdownOpen = false; // close strategy dropdown when opening deck dropdown
        }
        if (deckDropdownOpen) {
            for (std::size_t i = 0; i < deckDropdownOptions.size(); ++i) {
                if (deckDropdownOptions[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    deckDropdownSelectedIndex = i;
                    deckDropdownText.setString(amountOfDecks[i]);
                    deckDropdownOpen = false;
                    if (i == 0)      gameSettings[3] = 1;
                    else if (i == 1) gameSettings[3] = 6;
                    else if (i == 2) gameSettings[3] = 8;
                    break;
                }
            }
        }
    }

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isDraggingSlider = false;
        isDraggingSlider2 = false;
        isDraggingSlider3 = false;
    }
}

PlayMenu::Option PlayMenu::getSelectedOption() const {
    switch (selectedIndex) {
    case 0: return Option::BACK;
    case 1: return Option::START;
    default: return Option::NONE;
    }
}

sf::FloatRect PlayMenu::getSelectedOptionPos() const {
    return options[selectedIndex].getGlobalBounds();
}
