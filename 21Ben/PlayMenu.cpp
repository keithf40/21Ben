#include "PlayMenu.h"
#include <iostream>
#include <cmath>  // For std::round

// Constructor - initializes the "Back" and "Play" buttons, the checkbox, the sliders, and the dropdown
PlayMenu::PlayMenu(float width, float height)
    : selectedIndex(0), isChecked(false), isDraggingSlider(false), isDraggingSlider2(false),
    dropdownOpen(false), dropdownSelectedIndex(0)
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
    // Position it at a chosen location
    checkbox.setPosition(sf::Vector2f(width / 3.9f, height / 2.5f));
    checkbox.setFillColor(sf::Color::White);

    // Initialize the checkbox label
    checkboxLabel.setFont(font);
    checkboxLabel.setString("Enable Assistance");
    checkboxLabel.setCharacterSize(30);
    checkboxLabel.setFillColor(sf::Color::White);
    // Position label next to the checkbox
    checkboxLabel.setPosition(checkbox.getPosition().x + 30.f, checkbox.getPosition().y - 5.f);

    // --------------------------
    // Initialize the dropdown menu for strategy selection:
    // Move it to below the enable assistance checkbox
    dropdownBox.setSize(sf::Vector2f(150.f, 30.f));
    dropdownBox.setPosition(checkbox.getPosition().x, checkbox.getPosition().y + checkbox.getSize().y + 10.f);
    // If assistance is off, the dropdown is disabled (gray)
    dropdownBox.setFillColor(isChecked ? sf::Color::White : sf::Color(128, 128, 128));

    dropdownText.setFont(font);
    dropdownText.setString(selectedStrategy);
    dropdownText.setCharacterSize(20);
    dropdownText.setFillColor(sf::Color::Black);
    dropdownText.setPosition(dropdownBox.getPosition().x + 5.f, dropdownBox.getPosition().y + 5.f);

    // Create dropdown options (positioned below the dropdownBox)
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
    // Move the two sliders to the right of the enable assistance area.
    // Compute an x-coordinate offset based on the checkbox label width.
    float sliderX = checkboxLabel.getPosition().x + checkboxLabel.getLocalBounds().width + 50.f;

    // Initialize the slider for minimum bet value:
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

    // Initialize the second slider for starting money:
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
}

// Draw all the buttons, the checkbox, sliders, and the dropdown menu
void PlayMenu::draw(sf::RenderWindow& window) {
    // Draw menu options
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

    // Draw dropdown box and text
    window.draw(dropdownBox);
    window.draw(dropdownText);

    // If the dropdown is open and assistance is enabled, draw the white background box behind the dropdown options
    if (dropdownOpen && isChecked) {
        // Calculate the background rectangle covering all dropdown options
        sf::RectangleShape dropdownBackground;
        float bgX = dropdownBox.getPosition().x;
        float bgY = dropdownBox.getPosition().y + dropdownBox.getSize().y;
        float bgWidth = dropdownBox.getSize().x;
        float bgHeight = dropdownBox.getSize().y * static_cast<float>(dropdownOptions.size());
        dropdownBackground.setSize(sf::Vector2f(bgWidth, bgHeight));
        dropdownBackground.setPosition(bgX, bgY);
        dropdownBackground.setFillColor(sf::Color::White);
        window.draw(dropdownBackground);

        // Now draw each dropdown option on top of the white background
        for (const auto& option : dropdownOptions) {
            window.draw(option);
        }
    }
}

// Handle events: updates for buttons, checkbox, sliders, and the dropdown menu
void PlayMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    // Update dropdown appearance based on whether assistance is enabled
    if (!isChecked)
        dropdownBox.setFillColor(sf::Color(128, 128, 128));
    else
        dropdownBox.setFillColor(sf::Color::White);

    // Handle mouse movement events
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // If the first slider is being dragged, update its knob and minimum bet value
        if (isDraggingSlider) {
            float newX = static_cast<float>(mousePos.x);
            float leftBound = sliderTrack.getPosition().x;
            float rightBound = leftBound + sliderTrack.getSize().x;
            if (newX < leftBound) newX = leftBound;
            if (newX > rightBound) newX = rightBound;
            sliderKnob.setPosition(newX - sliderKnob.getSize().x / 2, sliderKnob.getPosition().y);
            float ratio = (newX - leftBound) / sliderTrack.getSize().x;
            double raw = 15 + ratio * (200 - 15);
            int quantized = 15 + static_cast<int>(std::round((raw - 15) / 5.0)) * 5;
            if (quantized > 200) quantized = 200;
            gameSettings[1] = quantized;
            sliderLabel.setString("Min Bet: " + std::to_string(gameSettings[1]));
        }

        // If the second slider is being dragged, update its knob and starting money value
        if (isDraggingSlider2) {
            float newX = static_cast<float>(mousePos.x);
            float leftBound = slider2Track.getPosition().x;
            float rightBound = leftBound + slider2Track.getSize().x;
            if (newX < leftBound) newX = leftBound;
            if (newX > rightBound) newX = rightBound;
            slider2Knob.setPosition(newX - slider2Knob.getSize().x / 2, slider2Knob.getPosition().y);
            float ratio2 = (newX - leftBound) / slider2Track.getSize().x;
            double raw2 = 100 + ratio2 * (5000 - 100);
            int quantized2 = 100 + static_cast<int>(std::round((raw2 - 100) / 50.0)) * 50;
            if (quantized2 > 5000) quantized2 = 5000;
            gameSettings[2] = quantized2;
            slider2Label.setString("Starting Money: " + std::to_string(gameSettings[2]));
        }

        // Handle highlighting for menu options (Back/Play)
        for (std::size_t i = 0; i < options.size(); ++i) {
            if (options[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                options[i].setFillColor(sf::Color::Yellow);
                selectedIndex = i;
            }
            else {
                options[i].setFillColor(sf::Color::White);
            }
        }

        // If dropdown is open and assistance is enabled, highlight dropdown options on hover
        if (dropdownOpen && isChecked) {
            for (auto& option : dropdownOptions) {
                if (option.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                    option.setFillColor(sf::Color::Yellow);
                else
                    option.setFillColor(sf::Color::Black);
            }
        }
    }

    // Handle mouse button pressed events
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Checkbox toggle
        if (checkbox.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            isChecked = !isChecked;
            gameSettings[0] = isChecked ? 1 : 0;
            checkbox.setFillColor(isChecked ? sf::Color::Green : sf::Color::White);
            // When assistance is turned off, ensure the dropdown closes
            if (!isChecked) {
                dropdownOpen = false;
            }
        }

        // First slider: check if clicked on knob or track
        if (sliderKnob.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)) ||
            sliderTrack.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            isDraggingSlider = true;
            float newX = static_cast<float>(mousePos.x);
            float leftBound = sliderTrack.getPosition().x;
            float rightBound = leftBound + sliderTrack.getSize().x;
            if (newX < leftBound) newX = leftBound;
            if (newX > rightBound) newX = rightBound;
            sliderKnob.setPosition(newX - sliderKnob.getSize().x / 2, sliderKnob.getPosition().y);
            float ratio = (newX - leftBound) / sliderTrack.getSize().x;
            double raw = 15 + ratio * (200 - 15);
            int quantized = 15 + static_cast<int>(std::round((raw - 15) / 5.0)) * 5;
            if (quantized > 200) quantized = 200;
            gameSettings[1] = quantized;
            sliderLabel.setString("Min Bet: " + std::to_string(gameSettings[1]));
        }

        // Second slider: check if clicked on knob or track
        if (slider2Knob.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)) ||
            slider2Track.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            isDraggingSlider2 = true;
            float newX = static_cast<float>(mousePos.x);
            float leftBound = slider2Track.getPosition().x;
            float rightBound = leftBound + slider2Track.getSize().x;
            if (newX < leftBound) newX = leftBound;
            if (newX > rightBound) newX = rightBound;
            slider2Knob.setPosition(newX - slider2Knob.getSize().x / 2, slider2Knob.getPosition().y);
            float ratio2 = (newX - leftBound) / slider2Track.getSize().x;
            double raw2 = 100 + ratio2 * (5000 - 100);
            int quantized2 = 100 + static_cast<int>(std::round((raw2 - 100) / 50.0)) * 50;
            if (quantized2 > 5000) quantized2 = 5000;
            gameSettings[2] = quantized2;
            slider2Label.setString("Starting Money: " + std::to_string(gameSettings[2]));
        }

        // Dropdown interaction: only if assistance is enabled
        if (isChecked) {
            // Toggle dropdown if clicking on the dropdown box
            if (dropdownBox.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                dropdownOpen = !dropdownOpen;
            }
            // If dropdown is open, check if one of the options was clicked
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
    }

    // Handle mouse button release events
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isDraggingSlider = false;
        isDraggingSlider2 = false;
    }
}

// Get the currently selected menu option (Back/Play)
PlayMenu::Option PlayMenu::getSelectedOption() const {
    switch (selectedIndex) {
    case 0: return Option::BACK;
    case 1: return Option::START;
    default: return Option::NONE;
    }
}

// Return the bounds of the currently selected option
sf::FloatRect PlayMenu::getSelectedOptionPos() const {
    return options[selectedIndex].getGlobalBounds();
}
