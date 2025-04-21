#include "StatisticsMenu.h"
#include "StatsTracker.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>

StatisticsMenu::StatisticsMenu(float width, float height) : selectedOption(Option::NONE), width(width), height(height) {
    // Load font
    if (!font.loadFromFile("assets/ttfFont.ttf")) {
        throw std::runtime_error("Error loading font");
    }

    // Setup background - change to dark with transparency
    background.setSize(sf::Vector2f(width, height));
    background.setFillColor(sf::Color(20, 20, 20, 220)); // Dark background with transparency

    // Setup title - make it larger and more stylized
    title.setFont(font);
    title.setString("Lifetime Statistics");
    title.setCharacterSize(60);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    title.setPosition(width / 2 - title.getGlobalBounds().width / 2, 30);

    // Setup back button - match the home page style
    backButton.setFont(font);
    backButton.setString("Back to Main Menu");
    backButton.setCharacterSize(40);
    backButton.setFillColor(sf::Color::White);
    backButton.setPosition(50, height - 80);

    // Create main content panel
    contentPanel.setSize(sf::Vector2f(900, 550));
    contentPanel.setFillColor(sf::Color(40, 40, 40, 230)); // Slightly lighter than background
    contentPanel.setOutlineColor(sf::Color(255, 215, 0)); // Gold outline
    contentPanel.setOutlineThickness(3);
    contentPanel.setPosition((width - contentPanel.getSize().x) / 2, 120);

    // Load and create stat texts
    loadStats();
    createStatTexts();
}

StatisticsMenu::~StatisticsMenu() {}

void StatisticsMenu::loadStats() {
    // Use the StatsTracker singleton to access real statistics
    StatsTracker& tracker = StatsTracker::getInstance();

    // Copy the stats from the tracker to our local structure
    stats.totalGamesPlayed = tracker.getTotalGamesPlayed();
    stats.totalWins = tracker.getTotalWins();
    stats.totalLosses = tracker.getTotalLosses();
    stats.totalDraws = tracker.getTotalDraws();
    stats.winPercentage = tracker.getWinPercentage();

    stats.totalChipsWon = tracker.getTotalChipsWon();
    stats.totalChipsLost = tracker.getTotalChipsLost();
    stats.avgBetSize = tracker.getAvgBetSize();
    stats.maxWinStreak = tracker.getMaxWinStreak();
    stats.maxLossStreak = tracker.getMaxLossStreak();

    // Clear previous strategies
    stats.usedStrategies.clear();

    // Add all strategies even if they haven't been used
    stats.usedStrategies = {
        "Standard",   // Basic strategy
        "HiLo",       // Hi-Lo counting
        "KO",         // Knockout counting
        "HiOpt1",     // Hi-Opt I counting
        "HiOpt2",     // Hi-Opt II counting
        "Mentor",     // Mentor counting
        "Omega2",     // Omega II counting
        "RPC",        // Red-7 Point Count
        "RAPC",       // Revere Advanced Point Count
        "R14C",       // Revere 14-Count
        "WongHalves", // Wong Halves counting
        "Zen"         // Zen Count
    };

    // Get win rates and avg chips for each strategy (used or not)
    for (const auto& strategy : stats.usedStrategies) {
        stats.strategyWinRates[strategy] = tracker.getStrategyWinRate(strategy);
        stats.strategyAvgChips[strategy] = tracker.getStrategyAvgChips(strategy);
    }
}

std::string StatisticsMenu::formatStat(const std::string& label, const std::string& value) {
    return label + ": " + value;
}

std::string StatisticsMenu::formatFloat(float value) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << value;
    return ss.str();
}

void StatisticsMenu::createStatTexts() {
    statTexts.clear();

    // Calculate positions relative to the content panel
    float panelX = contentPanel.getPosition().x;
    float panelY = contentPanel.getPosition().y;
    float panelWidth = contentPanel.getSize().x;

    float startY = panelY + 30;
    float lineHeight = 40;
    float leftColumnX = panelX + 40;
    float middleColumnX = panelX + panelWidth / 3 + 20;
    float rightColumnX = panelX + 2 * panelWidth / 3 + 20;

    // Create section headers first
    createSectionHeaders(leftColumnX, middleColumnX, rightColumnX, startY);

    // Create overall stats at the left
    createOverallStatsTexts(startY + 60, lineHeight, leftColumnX);

    // Create betting stats in the middle
    createBettingStatsTexts(startY + 60, lineHeight, middleColumnX);

    // Create strategy stats on the right
    createStrategyStatsTexts(startY + 60, lineHeight, rightColumnX);
}

void StatisticsMenu::createSectionHeaders(float leftX, float middleX, float rightX, float y) {
    // Helper lambda to add a section header
    auto addSectionHeader = [this](const std::string& text, float x, float y) {
        sf::Text headerText;
        headerText.setFont(font);
        headerText.setString(text);
        headerText.setCharacterSize(32);
        headerText.setFillColor(sf::Color(255, 215, 0)); // Gold color
        headerText.setStyle(sf::Text::Bold | sf::Text::Underlined);
        headerText.setPosition(x, y);
        statTexts.push_back(headerText);
        };

    addSectionHeader("Overall Results", leftX, y);
    addSectionHeader("Betting Performance", middleX, y);
    addSectionHeader("Strategy Comparison", rightX, y);
}

void StatisticsMenu::createOverallStatsTexts(float startY, float lineHeight, float columnX) {
    // Helper lambda to add a stat text
    auto addStatText = [this](const std::string& text, float x, float y) {
        sf::Text statText;
        statText.setFont(font);
        statText.setString(text);
        statText.setCharacterSize(24);
        statText.setFillColor(sf::Color::White);
        statText.setPosition(x, y);
        statTexts.push_back(statText);
        };

    // Overall Results section
    addStatText(formatStat("Total Games Played", std::to_string(stats.totalGamesPlayed)), columnX, startY);
    addStatText(formatStat("Total Wins", std::to_string(stats.totalWins)), columnX, startY + lineHeight);
    addStatText(formatStat("Total Losses", std::to_string(stats.totalLosses)), columnX, startY + 2 * lineHeight);
    addStatText(formatStat("Total Draws", std::to_string(stats.totalDraws)), columnX, startY + 3 * lineHeight);

    // Highlight win percentage with a different color
    sf::Text winPercentageText;
    winPercentageText.setFont(font);
    winPercentageText.setString(formatStat("Win Percentage", formatFloat(stats.winPercentage) + "%"));
    winPercentageText.setCharacterSize(28);
    winPercentageText.setFillColor(sf::Color(255, 255, 0)); // Bright yellow
    winPercentageText.setPosition(columnX, startY + 4 * lineHeight + 10);
    statTexts.push_back(winPercentageText);
}

void StatisticsMenu::createBettingStatsTexts(float startY, float lineHeight, float columnX) {
    // Helper lambda to add a stat text
    auto addStatText = [this](const std::string& text, float x, float y) {
        sf::Text statText;
        statText.setFont(font);
        statText.setString(text);
        statText.setCharacterSize(24);
        statText.setFillColor(sf::Color::White);
        statText.setPosition(x, y);
        statTexts.push_back(statText);
        };

    // Betting Performance section
    addStatText(formatStat("Total Chips Won", std::to_string(stats.totalChipsWon)), columnX, startY);
    addStatText(formatStat("Total Chips Lost", std::to_string(stats.totalChipsLost)), columnX, startY + lineHeight);
    addStatText(formatStat("Average Bet Size", "$" + formatFloat(stats.avgBetSize)), columnX, startY + 2 * lineHeight);

    // Highlight streak stats
    sf::Text winStreakText;
    winStreakText.setFont(font);
    winStreakText.setString(formatStat("Max Win Streak", std::to_string(stats.maxWinStreak)));
    winStreakText.setCharacterSize(24);
    winStreakText.setFillColor(sf::Color(100, 255, 100)); // Green
    winStreakText.setPosition(columnX, startY + 3 * lineHeight);
    statTexts.push_back(winStreakText);

    sf::Text lossStreakText;
    lossStreakText.setFont(font);
    lossStreakText.setString(formatStat("Max Loss Streak", std::to_string(stats.maxLossStreak)));
    lossStreakText.setCharacterSize(24);
    lossStreakText.setFillColor(sf::Color(255, 100, 100)); // Red
    lossStreakText.setPosition(columnX, startY + 4 * lineHeight);
    statTexts.push_back(lossStreakText);
}

void StatisticsMenu::createStrategyStatsTexts(float startY, float lineHeight, float columnX) {
    // Helper lambda to add a stat text
    auto addStatText = [this](const std::string& text, float x, float y, const sf::Color& color = sf::Color::White) {
        sf::Text statText;
        statText.setFont(font);
        statText.setString(text);
        statText.setCharacterSize(22);
        statText.setFillColor(color);
        statText.setPosition(x, y);
        statTexts.push_back(statText);
        };

    // Strategy Comparison section - add table headers
    addStatText("Strategy", columnX, startY, sf::Color(180, 180, 180));
    addStatText("Win Rate", columnX + 90, startY, sf::Color(180, 180, 180));
    addStatText("Avg Chips", columnX + 170, startY, sf::Color(180, 180, 180));

    // Add horizontal separator line
    sf::RectangleShape headerLine(sf::Vector2f(240, 2));
    headerLine.setFillColor(sf::Color(200, 200, 200));
    headerLine.setPosition(columnX, startY + 25);
    headerLines.push_back(headerLine);

    float yPos = startY + 35;

    // Add data for each strategy, applying scroll offset if needed
    int displayedStrategies = 0;
    const int maxDisplayStrategies = 8; // Maximum strategies to show at once

    for (size_t i = 0; i < stats.usedStrategies.size(); i++) {
        const float currentY = yPos + (displayedStrategies * lineHeight) - scrollOffset;

        // Skip if outside visible area after applying scroll
        if (currentY < startY + 35 || currentY > contentPanel.getPosition().y + contentPanel.getSize().y - 60) {
            continue;
        }

        if (displayedStrategies >= maxDisplayStrategies) {
            canScrollDown = true;
            break;
        }

        const auto& strategy = stats.usedStrategies[i];

        // Highlight the active strategy
        sf::Color textColor = (i == 0) ? sf::Color(255, 215, 0) : sf::Color::White;
        addStatText(strategy, columnX, currentY, textColor);

        // Win rate - display "N/A" if no games played with this strategy
        std::string winRateStr;
        if (stats.strategyWinRates[strategy] == 0.0f && stats.totalGamesPlayed > 0) {
            // Check if the strategy has actually been used
            bool strategyUsed = false;
            for (const auto& usedStrategy : StatsTracker::getInstance().getUsedStrategies()) {
                if (strategy == usedStrategy) {
                    strategyUsed = true;
                    break;
                }
            }

            winRateStr = strategyUsed ? "0.0%" : "N/A";
        }
        else {
            winRateStr = formatFloat(stats.strategyWinRates[strategy]) + "%";
        }
        addStatText(winRateStr, columnX + 90, currentY, textColor);

        // Avg chips - display "N/A" if no games played with this strategy
        std::string avgChipsStr;
        if (stats.strategyAvgChips[strategy] == 0.0f && stats.totalGamesPlayed > 0) {
            // Check if the strategy has actually been used
            bool strategyUsed = false;
            for (const auto& usedStrategy : StatsTracker::getInstance().getUsedStrategies()) {
                if (strategy == usedStrategy) {
                    strategyUsed = true;
                    break;
                }
            }

            avgChipsStr = strategyUsed ? "$0.0" : "N/A";
        }
        else {
            avgChipsStr = "$" + formatFloat(stats.strategyAvgChips[strategy]);
        }
        addStatText(avgChipsStr, columnX + 170, currentY, textColor);

        displayedStrategies++;
    }

    // Add scroll indicators if needed
    if (scrollOffset > 0) {
        canScrollUp = true;
        sf::Text scrollUpText;
        scrollUpText.setFont(font);
        scrollUpText.setString(" More ");
        scrollUpText.setCharacterSize(20);
        scrollUpText.setFillColor(sf::Color::Yellow);
        scrollUpText.setPosition(columnX + 90, startY - 25);
        statTexts.push_back(scrollUpText);
    }
    else {
        canScrollUp = false;
    }

    if (canScrollDown) {
        sf::Text scrollDownText;
        scrollDownText.setFont(font);
        scrollDownText.setString(" More ");
        scrollDownText.setCharacterSize(20);
        scrollDownText.setFillColor(sf::Color::Yellow);
        scrollDownText.setPosition(columnX + 90, contentPanel.getPosition().y + contentPanel.getSize().y - 40);
        statTexts.push_back(scrollDownText);
    }
}

void StatisticsMenu::draw(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(contentPanel);

    // Draw horizontal separator lines
    for (const auto& line : headerLines) {
        window.draw(line);
    }

    for (const auto& text : statTexts) {
        window.draw(text);
    }

    // Draw title and back button on top
    window.draw(title);
    window.draw(backButton);
}

void StatisticsMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            backButton.setFillColor(sf::Color::Yellow);
        }
        else {
            backButton.setFillColor(sf::Color::White);
        }
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            selectedOption = Option::BACK;
        }
    }

    // Handle scrolling with mouse wheel
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            if (event.mouseWheelScroll.delta > 0 && canScrollUp) {
                // Scroll up
                scrollOffset = std::max(0.0f, scrollOffset - 40.0f);
                createStatTexts();
            }
            else if (event.mouseWheelScroll.delta < 0 && canScrollDown) {
                // Scroll down
                scrollOffset += 40.0f;
                createStatTexts();
            }
        }
    }

    // Handle scrolling with keyboard
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up && canScrollUp) {
            scrollOffset = std::max(0.0f, scrollOffset - 40.0f);
            createStatTexts();
        }
        else if (event.key.code == sf::Keyboard::Down && canScrollDown) {
            scrollOffset += 40.0f;
            createStatTexts();
        }
    }
}

StatisticsMenu::Option StatisticsMenu::getSelectedOption() const {
    return selectedOption;
}

void StatisticsMenu::updateStats() {
    scrollOffset = 0.0f; // Reset scroll position
    loadStats();
    createStatTexts();
}