#include "SimulationResults.h"
#include <iostream>

SimulationResults::SimulationResults(float width, float height) {
    resultBox.setSize(sf::Vector2f(width * 0.8f, height * 0.8f));
    resultBox.setFillColor(sf::Color(30, 30, 30)); // Dark dark gray
    resultBox.setOutlineColor(sf::Color::Yellow);
    resultBox.setOutlineThickness(4.f);
    resultBox.setPosition(width * 0.1f, height * 0.1f);

    if (!font.loadFromFile("assets/ttfFont.ttf")) {
        std::cerr << "Font not found!" << std::endl;
    }

    // Back button setup (bottom left, same as PlayMenu)
    backButton.setFont(font);
    backButton.setString("Back");
    backButton.setCharacterSize(50);
    backButton.setFillColor(sf::Color::White);
    backButton.setPosition(10.f, height / 1.1f);
}

void SimulationResults::draw(sf::RenderWindow& window) {
    window.draw(resultBox);
    window.draw(simInfoText);
    for (const auto& bar : barsStyle1) window.draw(bar);
    for (const auto& bar : barsStyle2) window.draw(bar);
    for (const auto& label : playerLabels) window.draw(label);
    window.draw(backButton);
    //window.draw(debugGainText);
    window.draw(xAxis);
    window.draw(yAxis);
    window.draw(yAxisLabelTop);
    window.draw(yAxisLabelZero);
    window.draw(yAxisLabelBottom);
    window.draw(legendBox1);
    window.draw(legendText1);
    if (style2Name != "N/A") {
        window.draw(legendBox2);
        window.draw(legendText2);
    }
}

bool SimulationResults::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    bool hovering = backButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    backButton.setFillColor(hovering ? sf::Color::Yellow : sf::Color::White);

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (hovering) return true;
    }
    return false;
}

void SimulationResults::setSimulationInfo(int rounds, int handsPerRound, std::string playstyle1, std::string playstyle2) {
    simInfoText.setFont(font);
    simInfoText.setCharacterSize(30);
    simInfoText.setFillColor(sf::Color::White);
    simInfoText.setString("Rounds Simulated: " + std::to_string(rounds) +
        " | Hands per Round: " + std::to_string(handsPerRound));
    // Center the text at the top of resultBox
    sf::FloatRect textBounds = simInfoText.getLocalBounds();
    float centerX = resultBox.getPosition().x + resultBox.getSize().x / 2.f;
    simInfoText.setOrigin(textBounds.width / 2.f, 0.f);
    simInfoText.setPosition(centerX, resultBox.getPosition().y + 10.f + simInfoText.getCharacterSize());
}

void SimulationResults::setAverageGains(const std::vector<long long>& gains1, const std::vector<long long>& gains2, const std::string& style1, const std::string& style2, int startingMoney) {
    barsStyle1.clear();
    barsStyle2.clear();
    playerLabels.clear();
    moneyCap = startingMoney;
    style2Name = style2;

    float graphWidth = resultBox.getSize().x * 0.75f;
    float graphHeight = resultBox.getSize().y * 0.6f;
    float graphX = resultBox.getPosition().x + resultBox.getSize().x * 0.1f;
    float graphY = resultBox.getPosition().y + resultBox.getSize().y * 0.25f;

    float groupSpacing = graphWidth / 5.f;
    float barSpacing = 2.f;
    float barWidth = 40.f;
    float totalBarWidth = (style2 != "N/A") ? (2 * barWidth + barSpacing) : barWidth;

    float graphBottom = graphY + graphHeight; // y-position of -startingMoney
    float base = -moneyCap; // Used to normalize gain to [0, 2*moneyCap]

    for (int i = 0; i < 5; ++i) {
        float groupX = graphX + groupSpacing * i + (groupSpacing - totalBarWidth) / 2.f;

        // Yellow bar (style1)
        float normalized1 = static_cast<float>(gains1[i] - base) / (2.f * moneyCap);
        float height1 = std::max(0.f, normalized1 * graphHeight);
        sf::RectangleShape bar1(sf::Vector2f(barWidth, height1));
        bar1.setFillColor(sf::Color::Yellow);
        bar1.setPosition(groupX, graphBottom - height1);
        barsStyle1.push_back(bar1);

        // Green bar (style2)
        if (style2 != "N/A") {
            float normalized2 = static_cast<float>(gains2[i] - base) / (2.f * moneyCap);
            float height2 = std::max(0.f, normalized2 * graphHeight);
            sf::RectangleShape bar2(sf::Vector2f(barWidth, height2));
            bar2.setFillColor(sf::Color::Green);
            bar2.setPosition(groupX + barWidth + barSpacing, graphBottom - height2);
            barsStyle2.push_back(bar2);
        }

        // Player label (centered under the group)
        sf::Text label;
        label.setFont(font);
        label.setCharacterSize(20);
        label.setFillColor(sf::Color::White);
        label.setString("P" + std::to_string(i + 1));
        sf::FloatRect labelBounds = label.getLocalBounds();
        label.setOrigin(labelBounds.width / 2.f, 0.f);
        label.setPosition(groupX + totalBarWidth / 2.f, graphBottom + 5.f);
        playerLabels.push_back(label);
    }

    // Debug display for Player 3
    long long gain1 = gains1[2];
    long long gain2 = (style2 != "N/A") ? gains2[2] : 0;
    debugGainText.setFont(font);
    debugGainText.setCharacterSize(24);
    debugGainText.setFillColor(sf::Color::White);
    if (style2 != "N/A") {
        debugGainText.setString("P3 Avg Gain — " + style1 + ": " + std::to_string(gain1) +
            ", " + style2 + ": " + std::to_string(gain2));
    }
    else {
        debugGainText.setString("P3 Avg Gain — " + style1 + ": " + std::to_string(gain1));
    }
    sf::FloatRect textBounds = debugGainText.getLocalBounds();
    float centerX = resultBox.getPosition().x + resultBox.getSize().x / 2.f;
    debugGainText.setOrigin(textBounds.width / 2.f, 0.f);
    debugGainText.setPosition(centerX, simInfoText.getPosition().y + simInfoText.getCharacterSize() + 10.f);

    // Axes
    xAxis = sf::VertexArray(sf::Lines, 2);
    yAxis = sf::VertexArray(sf::Lines, 2);

    // X-axis (drawn at graph bottom)
    xAxis[0].position = sf::Vector2f(graphX, graphBottom);
    xAxis[0].color = sf::Color::White;
    xAxis[1].position = sf::Vector2f(graphX + graphWidth, graphBottom);
    xAxis[1].color = sf::Color::White;

    // Y-axis (optional)
    yAxis[0].position = sf::Vector2f(graphX, graphY);
    yAxis[0].color = sf::Color::White;
    yAxis[1].position = sf::Vector2f(graphX, graphBottom);
    yAxis[1].color = sf::Color::White;

    // Y-axis Labels
    yAxisLabelTop.setFont(font);
    yAxisLabelTop.setCharacterSize(18);
    yAxisLabelTop.setFillColor(sf::Color::White);
    yAxisLabelTop.setString("+" + std::to_string(moneyCap));
    yAxisLabelTop.setPosition(graphX - 60.f, graphY - 20.f);

    yAxisLabelZero.setFont(font);
    yAxisLabelZero.setCharacterSize(18);
    yAxisLabelZero.setFillColor(sf::Color::White);
    yAxisLabelZero.setString("0");
    yAxisLabelZero.setPosition(graphX - 30.f,
        graphBottom - ((0 - base) / (2.f * moneyCap)) * graphHeight - 10.f);

    yAxisLabelBottom.setFont(font);
    yAxisLabelBottom.setCharacterSize(18);
    yAxisLabelBottom.setFillColor(sf::Color::White);
    yAxisLabelBottom.setString("-" + std::to_string(moneyCap));
    yAxisLabelBottom.setPosition(graphX - 60.f, graphBottom - 20.f);
    
    // Legend setup
    float legendBoxSize = 15.f;
    float legendPadding = 10.f;

    float legendX = graphX + graphWidth - 100.f; // near top-right of graph
    float legendY = graphY - 30.f;               // above graph

    // Yellow box + text (style1)
    legendBox1.setSize(sf::Vector2f(legendBoxSize, legendBoxSize));
    legendBox1.setFillColor(sf::Color::Yellow);
    legendBox1.setPosition(legendX, legendY);

    legendText1.setFont(font);
    legendText1.setCharacterSize(18);
    legendText1.setFillColor(sf::Color::White);
    legendText1.setString(style1);
    legendText1.setPosition(legendX + legendBoxSize + 6.f, legendY - 2.f); // slight vertical nudge

    // Green box + text (style2, only if it's not "N/A")
    if (style2 != "N/A") {
        legendBox2.setSize(sf::Vector2f(legendBoxSize, legendBoxSize));
        legendBox2.setFillColor(sf::Color::Green);
        legendBox2.setPosition(legendX, legendY + legendBoxSize + legendPadding);

        legendText2.setFont(font);
        legendText2.setCharacterSize(18);
        legendText2.setFillColor(sf::Color::White);
        legendText2.setString(style2);
        legendText2.setPosition(legendX + legendBoxSize + 6.f, legendY + legendBoxSize + legendPadding - 2.f);
    }
    else {
        // If there's no second style, reset dimensions to hide them
        legendBox2.setSize(sf::Vector2f(0, 0));
        legendText2.setString("");
    }
}


