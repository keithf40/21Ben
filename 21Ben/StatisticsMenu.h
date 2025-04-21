#ifndef STATISTICS_MENU_H
#define STATISTICS_MENU_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <unordered_map>

class StatisticsMenu {
public:
    enum class Option { BACK, NONE };

    StatisticsMenu(float width, float height);
    ~StatisticsMenu();

    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    Option getSelectedOption() const;
    void updateStats(); // Updates the displayed statistics

private:
    struct LifetimeStats {
        int totalGamesPlayed = 0;
        int totalWins = 0;
        int totalLosses = 0;
        int totalDraws = 0;
        float winPercentage = 0.0f;
        int totalChipsWon = 0;
        int totalChipsLost = 0;
        float avgBetSize = 0.0f;
        int maxWinStreak = 0;
        int maxLossStreak = 0;

        // Map of strategy name to win rate
        std::unordered_map<std::string, float> strategyWinRates;
        // Map of strategy name to average chips
        std::unordered_map<std::string, float> strategyAvgChips;

        // List of strategies used
        std::vector<std::string> usedStrategies;
    };

    void loadStats(); // Loads statistics from file
    void createStatTexts(); // Creates and positions stat text elements
    void createSectionHeaders(float leftX, float middleX, float rightX, float y);
    void createOverallStatsTexts(float startY, float lineHeight, float columnX);
    void createBettingStatsTexts(float startY, float lineHeight, float columnX);
    void createStrategyStatsTexts(float startY, float lineHeight, float columnX);
    std::string formatStat(const std::string& label, const std::string& value);
    std::string formatFloat(float value);

    sf::Font font;
    std::vector<sf::Text> statTexts;
    sf::Text backButton;
    LifetimeStats stats;
    Option selectedOption;

    // Background elements
    sf::RectangleShape background;
    sf::RectangleShape contentPanel;
    std::vector<sf::RectangleShape> headerLines;
    sf::Text title;

    // Screen dimensions
    float width;
    float height;

    // For scrolling (if many strategies)
    float scrollOffset = 0.0f;
    bool canScrollDown = false;
    bool canScrollUp = false;
};

#endif // STATISTICS_MENU_H 