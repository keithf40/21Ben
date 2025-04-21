#include "SimulateMenu.h"
#include <iostream>
#include <cmath>  // For std::round

// Constructor - initializes buttons, checkbox, sliders, dropdowns and the new "Select Playstyle" dropdown,
// and now uses a slider for deck count instead of a dropdown.
SimulateMenu::SimulateMenu(float width, float height)
    : selectedIndex(0), isChecked(false), isDraggingSlider(false), isDraggingSlider2(false),
    dropdownOpen(false), dropdownSelectedIndex(0),
    isDraggingSlider3(false), playstyleDropdownOpen(false), playstyleDropdownSelectedIndex(0),
    isDraggingDeckSlider(false) // initialize deck slider flag
{
    // Load the font used for menu text
    if (!font.loadFromFile("assets/ttfFont.ttf")) {
        std::cerr << "Font not found!" << std::endl;
    }

    // Title
    titleText.setFont(font);
    titleText.setString("Simulation Settings");
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

    // Create "Simulate" button
    sf::Text playButton;
    playButton.setFont(font);
    playButton.setString("Simulate");
    playButton.setCharacterSize(35);
    playButton.setFillColor(sf::Color::White);
    playButton.setPosition(sf::Vector2f(width / 2.1f, height / 1.5f));

    options.push_back(backButton);
    options.push_back(playButton);

    // --------------------------
    // New "Select Playstyle" dropdown (always enabled)
    playstyleDropdownBox.setSize(sf::Vector2f(150.f, 30.f));
    playstyleDropdownBox.setPosition(sf::Vector2f(width / 3.9f, height / 2.5f));
    playstyleDropdownBox.setFillColor(sf::Color::White);

    playstyleDropdownText.setFont(font);
    playstyleDropdownText.setString(selectedStrategy);
    playstyleDropdownText.setCharacterSize(20);
    playstyleDropdownText.setFillColor(sf::Color::Black);
    playstyleDropdownText.setPosition(playstyleDropdownBox.getPosition().x + 5.f,
        playstyleDropdownBox.getPosition().y + 5.f);

    playstyleDropdownLabel.setFont(font);
    playstyleDropdownLabel.setString("Select Playstyle");
    playstyleDropdownLabel.setCharacterSize(20);
    playstyleDropdownLabel.setFillColor(sf::Color::White);
    playstyleDropdownLabel.setPosition(playstyleDropdownBox.getPosition().x,
        playstyleDropdownBox.getPosition().y - 30.f);

    for (std::size_t i = 0; i < strategies.size(); ++i) {
        sf::Text option;
        option.setFont(font);
        option.setString(strategies[i]);
        option.setCharacterSize(20);
        option.setFillColor(sf::Color::White);
        option.setPosition(playstyleDropdownBox.getPosition().x,
            playstyleDropdownBox.getPosition().y + playstyleDropdownBox.getSize().y * (i + 1));
        playstyleDropdownOptions.push_back(option);
    }

    // --------------------------
    // Initialize the checkbox for "Compare Playstyle to"
    checkbox.setSize(sf::Vector2f(20.f, 20.f));
    // Position: 40 pixels below the original playstyle dropdown position.
    checkbox.setPosition(sf::Vector2f(width / 3.9f, height / 2.5f + 40.f));
    checkbox.setFillColor(sf::Color::White);

    checkboxLabel.setFont(font);
    checkboxLabel.setString("Compare Playstyle to");
    checkboxLabel.setCharacterSize(20);
    checkboxLabel.setFillColor(sf::Color::White);
    checkboxLabel.setPosition(checkbox.getPosition().x + 30.f, checkbox.getPosition().y - 5.f);

    // --------------------------
    // Strategy dropdown for "Compare Playstyle to" (affected by assistance)
    dropdownBox.setSize(sf::Vector2f(150.f, 30.f));
    dropdownBox.setPosition(checkbox.getPosition().x, checkbox.getPosition().y + checkbox.getSize().y + 10.f);
    dropdownBox.setFillColor(isChecked ? sf::Color::White : sf::Color(128, 128, 128));

    dropdownText.setFont(font);
    dropdownText.setString(selectedStrategy2);
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
    // New Deck Count Slider (range 1-8) replacing the old deck dropdown
    // Determine a horizontal starting position similar to the other sliders.
    float sliderX = checkboxLabel.getPosition().x + checkboxLabel.getLocalBounds().width + 50.f;
    // Determine the original min bet slider Y position for reference.
    float minBetSliderY = checkbox.getPosition().y + 40.f;
    // Place deck slider 60 pixels above the min bet slider.
    deckSliderTrack.setSize(sf::Vector2f(250.f, 5.f));
    deckSliderTrack.setFillColor(sf::Color::White);
    deckSliderTrack.setPosition(sf::Vector2f(sliderX, minBetSliderY - 60.f));

    deckSliderKnob.setSize(sf::Vector2f(15.f, 25.f));
    deckSliderKnob.setFillColor(sf::Color::Yellow);
    {
        float ratio = float(gameSettings[3] - 1) / float(8 - 1);
        float knobX = deckSliderTrack.getPosition().x + ratio * deckSliderTrack.getSize().x;
        deckSliderKnob.setPosition(knobX - deckSliderKnob.getSize().x / 2,
            deckSliderTrack.getPosition().y - (deckSliderKnob.getSize().y - deckSliderTrack.getSize().y) / 2);
    }

    deckSliderLabel.setFont(font);
    deckSliderLabel.setString("Deck Count: " + std::to_string(gameSettings[3]));
    deckSliderLabel.setCharacterSize(25);
    deckSliderLabel.setFillColor(sf::Color::White);
    deckSliderLabel.setPosition(deckSliderTrack.getPosition().x, deckSliderTrack.getPosition().y - 40.f);

    // --------------------------
    // Slider for minimum bet:
    sliderTrack.setSize(sf::Vector2f(250.f, 5.f));
    sliderTrack.setFillColor(sf::Color::White);
    sliderTrack.setPosition(sf::Vector2f(sliderX, minBetSliderY));

    sliderKnob.setSize(sf::Vector2f(15.f, 25.f));
    sliderKnob.setFillColor(sf::Color::Yellow);
    {
        float ratio = float(gameSettings[1] - 5) / float(200 - 5);
        float knobX = sliderTrack.getPosition().x + ratio * sliderTrack.getSize().x;
        sliderKnob.setPosition(knobX - sliderKnob.getSize().x / 2,
            sliderTrack.getPosition().y - (sliderKnob.getSize().y - sliderTrack.getSize().y) / 2);
    }

    sliderLabel.setFont(font);
    sliderLabel.setString("Min Bet: " + std::to_string(gameSettings[1]));
    sliderLabel.setCharacterSize(25);
    sliderLabel.setFillColor(sf::Color::White);
    sliderLabel.setPosition(sliderTrack.getPosition().x, sliderTrack.getPosition().y - 40.f);

    // --------------------------
    // Slider for starting money:
    slider2Track.setSize(sf::Vector2f(250.f, 5.f));
    slider2Track.setFillColor(sf::Color::White);
    slider2Track.setPosition(sf::Vector2f(sliderX, sliderTrack.getPosition().y + 60.f));

    slider2Knob.setSize(sf::Vector2f(15.f, 25.f));
    slider2Knob.setFillColor(sf::Color::Yellow);
    {
        float ratio2 = float(gameSettings[2] - 100) / float(10000 - 100);
        float knob2X = slider2Track.getPosition().x + ratio2 * slider2Track.getSize().x;
        slider2Knob.setPosition(knob2X - slider2Knob.getSize().x / 2,
            slider2Track.getPosition().y - (slider2Knob.getSize().y - slider2Track.getSize().y) / 2);
    }

    slider2Label.setFont(font);
    slider2Label.setString("Buy-In: " + std::to_string(gameSettings[2]));
    slider2Label.setCharacterSize(25);
    slider2Label.setFillColor(sf::Color::White);
    slider2Label.setPosition(slider2Track.getPosition().x, slider2Track.getPosition().y - 40.f);

    // --------------------------
    // Slider for Player Position (range 1-5)
    slider3Track.setSize(sf::Vector2f(250.f, 5.f));
    slider3Track.setFillColor(sf::Color::White);
    slider3Track.setPosition(sf::Vector2f(sliderX, slider2Track.getPosition().y + 60.f));

    slider3Knob.setSize(sf::Vector2f(15.f, 25.f));
    slider3Knob.setFillColor(sf::Color::Yellow);
    {
        float ratio = float(gameSettings[4] - 100) / float(1000 - 100);
        float knobX = slider3Track.getPosition().x + ratio * slider3Track.getSize().x;
        slider3Knob.setPosition(knobX - slider3Knob.getSize().x / 2,
            slider3Track.getPosition().y - (slider3Knob.getSize().y - slider3Track.getSize().y) / 2);
    }

    slider3Label.setFont(font);
    slider3Label.setString("Simulations: " + std::to_string(gameSettings[4]));
    slider3Label.setCharacterSize(25);
    slider3Label.setFillColor(sf::Color::White);
    slider3Label.setPosition(slider3Track.getPosition().x, slider3Track.getPosition().y - 40.f);
}

// Draw all UI elements
void SimulateMenu::draw(sf::RenderWindow& window) {
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

    // Draw the "Select Playstyle" dropdown and its options (always enabled)
    window.draw(playstyleDropdownLabel);
    window.draw(playstyleDropdownBox);
    window.draw(playstyleDropdownText);
    if (playstyleDropdownOpen) {
        sf::RectangleShape playstyleDropdownBackground;
        float bgX = playstyleDropdownBox.getPosition().x;
        float bgY = playstyleDropdownBox.getPosition().y + playstyleDropdownBox.getSize().y;
        float bgWidth = playstyleDropdownBox.getSize().x;
        float bgHeight = playstyleDropdownBox.getSize().y * static_cast<float>(playstyleDropdownOptions.size());
        playstyleDropdownBackground.setSize(sf::Vector2f(bgWidth, bgHeight));
        playstyleDropdownBackground.setPosition(bgX, bgY);
        playstyleDropdownBackground.setFillColor(sf::Color::White);
        window.draw(playstyleDropdownBackground);
        for (const auto& option : playstyleDropdownOptions) {
            window.draw(option);
        }
    }

    // Reposition and draw strategy dropdown (for "Compare Playstyle to")
    if (playstyleDropdownOpen) {
        float newDropdownY = playstyleDropdownBox.getPosition().y +
            playstyleDropdownBox.getSize().y * (playstyleDropdownOptions.size() + 1) + 10.f;
        dropdownBox.setPosition(dropdownBox.getPosition().x, newDropdownY);
        dropdownText.setPosition(dropdownBox.getPosition().x + 5.f, dropdownBox.getPosition().y + 5.f);
        for (std::size_t i = 0; i < dropdownOptions.size(); ++i) {
            dropdownOptions[i].setPosition(
                dropdownBox.getPosition().x,
                dropdownBox.getPosition().y + dropdownBox.getSize().y * (i + 1)
            );
        }
    }
    else {
        dropdownBox.setPosition(checkbox.getPosition().x, checkbox.getPosition().y + checkbox.getSize().y + 10.f);
        dropdownText.setPosition(dropdownBox.getPosition().x + 5.f, dropdownBox.getPosition().y + 5.f);
        for (std::size_t i = 0; i < dropdownOptions.size(); ++i) {
            dropdownOptions[i].setPosition(
                dropdownBox.getPosition().x,
                dropdownBox.getPosition().y + dropdownBox.getSize().y * (i + 1)
            );
        }
    }
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

    // Draw the new Deck Count Slider
    window.draw(deckSliderTrack);
    window.draw(deckSliderKnob);
    window.draw(deckSliderLabel);

    // Draw the Player Position slider
    window.draw(slider3Track);
    window.draw(slider3Knob);
    window.draw(slider3Label);
}

// Handle events: updates for buttons, checkbox, sliders, and all dropdowns
void SimulateMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    // Update dropdown appearance based on the checkbox state
    if (!isChecked)
        dropdownBox.setFillColor(sf::Color(128, 128, 128));
    else
        dropdownBox.setFillColor(sf::Color::White);

    // Ensure playstyle dropdown box remains white.
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Update deck slider if dragging
        if (isDraggingDeckSlider) {
            float newX = static_cast<float>(mousePos.x);
            float leftBound = deckSliderTrack.getPosition().x;
            float rightBound = leftBound + deckSliderTrack.getSize().x;
            newX = (newX < leftBound) ? leftBound : (newX > rightBound ? rightBound : newX);
            deckSliderKnob.setPosition(newX - deckSliderKnob.getSize().x / 2, deckSliderKnob.getPosition().y);
            float ratio = (newX - leftBound) / deckSliderTrack.getSize().x;
            double raw = 1 + ratio * (8 - 1);
            int quantized = 1 + static_cast<int>(std::round(raw - 1));
            if (quantized < 1) quantized = 1;
            if (quantized > 8) quantized = 8;
            gameSettings[3] = quantized;
            deckSliderLabel.setString("Deck Count: " + std::to_string(gameSettings[3]));
        }

        // Update slider for minimum bet if dragged
        if (isDraggingSlider) {
            float newX = static_cast<float>(mousePos.x);
            float leftBound = sliderTrack.getPosition().x;
            float rightBound = leftBound + sliderTrack.getSize().x;
            newX = (newX < leftBound) ? leftBound : (newX > rightBound ? rightBound : newX);
            sliderKnob.setPosition(newX - sliderKnob.getSize().x / 2, sliderKnob.getPosition().y);
            float ratio = (newX - leftBound) / sliderTrack.getSize().x;
            double raw = 5 + ratio * (200 - 5);
            int quantized = 5 + static_cast<int>(std::round((raw - 5) / 5.0)) * 5;
            if (quantized < 5) quantized = 5;
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
            /*double raw2 = 100 + ratio2 * (5000 - 100);
            int quantized2 = 100 + static_cast<int>(std::round((raw2 - 100) / 50.0)) * 50;
            if (quantized2 > 5000) quantized2 = 5000;*/
            double raw2 = 100 + ratio2 * (10000 - 100);
            int quantized2 = 100 + static_cast<int>(std::round((raw2 - 100) / 50.0)) * 50;
            if (quantized2 > 10000) quantized2 = 10000;
            gameSettings[2] = quantized2;
            slider2Label.setString("Buy-In: " + std::to_string(gameSettings[2]));
        }

        // Update slider for Player Position if dragged
        if (isDraggingSlider3) {
            float newX = static_cast<float>(mousePos.x);
            float leftBound = slider3Track.getPosition().x;
            float rightBound = leftBound + slider3Track.getSize().x;
            if (newX < leftBound) newX = leftBound;
            if (newX > rightBound) newX = rightBound;
            slider3Knob.setPosition(newX - slider3Knob.getSize().x / 2, slider3Knob.getPosition().y);
            float ratio = (newX - leftBound) / slider3Track.getSize().x;
            double raw = 100 + ratio * (1000 - 100);
            int quantized = 100 + static_cast<int>(std::round((raw - 100) / 50.0)) * 50;
            if (quantized < 100) quantized = 100;
            if (quantized > 1000) quantized = 1000;
            gameSettings[4] = quantized;
            slider3Label.setString("Simulations: " + std::to_string(gameSettings[4]));
        }

        // Highlight Back/Simulate buttons
        for (std::size_t i = 0; i < options.size(); ++i) {
            if (options[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                options[i].setFillColor(sf::Color::Yellow);
                selectedIndex = i;
            }
            else {
                options[i].setFillColor(sf::Color::White);
            }
        }

        // Highlight playstyle dropdown options if open
        if (playstyleDropdownOpen) {
            for (auto& option : playstyleDropdownOptions) {
                option.setFillColor(option.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))
                    ? sf::Color::Yellow : sf::Color::Black);
            }
        }

        // Highlight strategy dropdown options if open and assistance is enabled
        if (dropdownOpen && isChecked) {
            for (auto& option : dropdownOptions) {
                option.setFillColor(option.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))
                    ? sf::Color::Yellow : sf::Color::Black);
            }
        }
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Playstyle dropdown interaction (always active)
        if (playstyleDropdownBox.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            playstyleDropdownOpen = !playstyleDropdownOpen;
            if (playstyleDropdownOpen)
                dropdownOpen = false;  // close other dropdowns when opening playstyle dropdown
        }
        if (playstyleDropdownOpen) {
            for (std::size_t i = 0; i < playstyleDropdownOptions.size(); ++i) {
                if (playstyleDropdownOptions[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    playstyleDropdownSelectedIndex = i;
                    selectedStrategy = strategies[i];
                    playstyleDropdownText.setString(selectedStrategy);
                    playstyleDropdownOpen = false;
                    break;
                }
            }
        }

        // Checkbox toggle
        if (checkbox.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            isChecked = !isChecked;
            gameSettings[0] = isChecked ? 1 : 0;
            checkbox.setFillColor(isChecked ? sf::Color::Green : sf::Color::White);
            if (!isChecked) {
                dropdownOpen = false;
            }
        }

        // Begin dragging the deck slider
        if (deckSliderKnob.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)) ||
            deckSliderTrack.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            isDraggingDeckSlider = true;
            float newX = static_cast<float>(mousePos.x);
            float leftBound = deckSliderTrack.getPosition().x;
            float rightBound = leftBound + deckSliderTrack.getSize().x;
            newX = (newX < leftBound) ? leftBound : (newX > rightBound ? rightBound : newX);
            deckSliderKnob.setPosition(newX - deckSliderKnob.getSize().x / 2, deckSliderKnob.getPosition().y);
            float ratio = (newX - leftBound) / deckSliderTrack.getSize().x;
            double raw = 1 + ratio * (8 - 1);
            int quantized = 1 + static_cast<int>(std::round(raw - 1));
            if (quantized < 1) quantized = 1;
            if (quantized > 8) quantized = 8;
            gameSettings[3] = quantized;
            deckSliderLabel.setString("Deck Count: " + std::to_string(gameSettings[3]));
        }

        // Slider interaction for minimum bet
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

        // Slider interaction for starting money
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
            slider2Label.setString("Buy-In: " + std::to_string(gameSettings[2]));
        }

        // Slider interaction for simulations
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
            double raw = 100 + ratio * (1000 - 100);
            int quantized = 100 + static_cast<int>(std::round((raw - 100) / 50.0)) * 50;
            if (quantized < 100) quantized = 100;
            if (quantized > 1000) quantized = 1000;
            gameSettings[4] = quantized;
            slider3Label.setString("Simulations: " + std::to_string(gameSettings[4]));
        }

        // Strategy dropdown interaction (only if assistance is enabled)
        if (isChecked) {
            if (dropdownBox.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                dropdownOpen = !dropdownOpen;
            }
            if (dropdownOpen) {
                for (std::size_t i = 0; i < dropdownOptions.size(); ++i) {
                    if (dropdownOptions[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        dropdownSelectedIndex = i;
                        selectedStrategy2 = strategies[i];
                        dropdownText.setString(selectedStrategy2);
                        dropdownOpen = false;
                        break;
                    }
                }
            }
        }
    }

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isDraggingSlider = false;
        isDraggingSlider2 = false;
        isDraggingSlider3 = false;
        isDraggingDeckSlider = false;
    }
}

SimulateMenu::Option SimulateMenu::getSelectedOption() const {
    switch (selectedIndex) {
    case 0: return Option::BACK;
    case 1: return Option::START;
    default: return Option::NONE;
    }
}

sf::FloatRect SimulateMenu::getSelectedOptionPos() const {
    return options[selectedIndex].getGlobalBounds();
}
