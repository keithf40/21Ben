#pragma once
#include <SFML/Graphics.hpp>

class SimulationResults {
public:
    SimulationResults(float width, float height);
    void draw(sf::RenderWindow& window);
    bool handleEvent(const sf::Event& event, sf::RenderWindow& window); // returns true if back clicked
    void setSimulationInfo(int rounds, int handsPerRound, std::string playstyle1, std::string playstyle2);
    void setAverageGains(const std::vector<long long>& gains1, const std::vector<long long>& gains2,
        const std::string& style1, const std::string& style2, int startingMoney);

private:
    sf::RectangleShape resultBox;
    sf::Font font;
    sf::Text backButton;
    sf::Text simInfoText;
    sf::Text debugGainText;
    std::vector<sf::RectangleShape> barsStyle1;
    std::vector<sf::RectangleShape> barsStyle2;
    std::vector<sf::Text> playerLabels;
    sf::VertexArray xAxis;
    sf::VertexArray yAxis;
    sf::Text yAxisLabelTop;
    sf::Text yAxisLabelZero;
    sf::Text yAxisLabelBottom;
    sf::RectangleShape legendBox1, legendBox2;
    sf::Text legendText1, legendText2;
    sf::Text legendTitle;
    sf::Text xAxisLabel;
    sf::Text yAxisLabel;
    std::vector<sf::Text> barValueLabels;
    int fixedBase = 100; // will be updated with actual startingMoney
    std::string style2Name = "N/A";
};
