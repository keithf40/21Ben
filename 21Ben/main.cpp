#include <SFML/Graphics.hpp>
#include "MainMenu.h"
#include "SimulateMenu.h"
#include "PlayMenu.h"
#include "Game.h"
#include "Texture.h"
#include "Simulation.h"
#include "SimulationResults.h"
#include "StatisticsMenu.h"
#include <memory> // For std::unique_ptr

// Enum to manage different game states
enum class GameState { MAIN_MENU, PLAY, SIMULATE, GAME, SIM_RESULTS, STATISTICS };

int main() {
    // Create main game window
    sf::RenderWindow window(sf::VideoMode({ 1200, 800 }), "Game");

    // Backgrounds for different screens
    sf::RectangleShape menuBackground(sf::Vector2f(1200.f, 800.f));
    sf::RectangleShape settingsBackground(sf::Vector2f(1200.f, 800.f));
    sf::RectangleShape gameBackground(sf::Vector2f(1200.f, 800.f));

    // Load textures for backgrounds
    Texture textures;
    textures.loadTextures();
    menuBackground.setTexture(&textures.menuBackgroundImage);
    settingsBackground.setTexture(&textures.settingsBackgroundImage);
    gameBackground.setTexture(&textures.gameBackgroundImage);

    // Initialize menus
    MainMenu mainMenu(1200, 800);
    SimulateMenu simulateMenu(1200, 800);
    PlayMenu playMenu(1200, 800);
    SimulationResults simResults(1200, 800);
    StatisticsMenu statisticsMenu(1200, 800);

    // Use a smart pointer for the game screen and delay its creation
    std::unique_ptr<Game> gameScreen = nullptr;

    // Start at the main menu
    GameState currState = GameState::MAIN_MENU;

    // Show the mouse cursor
    window.setMouseCursorVisible(true);

    // --- Create and configure the default settings text ---
    sf::Font font;
    if (!font.loadFromFile("assets/ttfFont.ttf")) {
        std::cerr << "Font not found!" << std::endl;
    }
    sf::Text defaultSettingsText;
    defaultSettingsText.setFont(font);
    defaultSettingsText.setString("Default Settings:\nSingle-Deck   Min. Bet: $15\nBuy-In: $100   Player Position: 1");
    defaultSettingsText.setCharacterSize(30);
    defaultSettingsText.setFillColor(sf::Color::White);
    // Define default game settings and strategy
    std::vector<int> defaultGameSettings = { 0, 15, 100, 1, 1 };
    std::string defaultStrategy = "Standard";
    // Instead of centering, set the origin to the bottom left and place with a small margin.
    sf::FloatRect textRect = defaultSettingsText.getLocalBounds();
    defaultSettingsText.setOrigin(0.f, textRect.height);
    defaultSettingsText.setPosition(10.f, window.getSize().y - 10.f);

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close the window
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Handle input based on current state
            switch (currState) {
            case GameState::MAIN_MENU:
                gameScreen.reset();
                mainMenu.handleEvent(event, window);
                if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    MainMenu::Option selected = mainMenu.getSelectedOption();
                    if (mainMenu.getSelectedOptionPos().contains(static_cast<sf::Vector2f>(mousePos))) {
                        if (selected == MainMenu::Option::PLAY) {
                            // Create a game using the default settings and immediately switch to GAME state.
                            gameScreen = std::make_unique<Game>(1200, 800, defaultStrategy, defaultGameSettings);
                            currState = GameState::GAME;
                        }
                        else if (selected == MainMenu::Option::GAME_SETTINGS) {
                            // Go to the game settings (Play Menu) state when "Game Settings" is clicked.
                            currState = GameState::PLAY;
                        }
                        else if (selected == MainMenu::Option::SIMULATE) {
                            currState = GameState::SIMULATE;
                        }
                        else if (selected == MainMenu::Option::EXIT) {
                            window.close();
                        }
                        else if (selected == MainMenu::Option::STATISTICS) {
                            statisticsMenu.updateStats();
                            currState = GameState::STATISTICS;
                        }
                    }
                }
                break;

            case GameState::SIMULATE:
                simulateMenu.handleEvent(event, window);
                if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    SimulateMenu::Option selected = simulateMenu.getSelectedOption();
                    if (simulateMenu.getSelectedOptionPos().contains(static_cast<sf::Vector2f>(mousePos))) {
                        if (selected == SimulateMenu::Option::BACK) {
                            currState = GameState::MAIN_MENU;
                        }
                        else if (selected == SimulateMenu::Option::START) {
                            // Retrieve simulation settings from simulateMenu.
                            std::vector<int> simSettings = simulateMenu.getGameSettings();
                            std::string playStyleOne = simulateMenu.getSelectedStrategy();
                            std::string playStyleTwo = simulateMenu.getSelectedStrategy2();

                            // Create and run the Simulation.
                            // simSettings[3]: deck size (amount of decks)
                            // simSettings[1]: minimum bet
                            // simSettings[2]: starting money
                            // simSettings[4]: simulations
                            if (simSettings[0] == 1) {  // Checkbox enabled: competing counts.
                                Simulation simulation(simSettings[3], simSettings[1], simSettings[2], simSettings[4], playStyleOne, playStyleTwo);
                                std::vector<std::vector<long long>> results;
                                auto simResultVector = simulation.Run(1000, simSettings[4]);
                                simResults.setSimulationInfo(simResultVector[0][0], simResultVector[0][1], playStyleOne, playStyleTwo);
                                simResults.setAverageGains(simResultVector[2], simResultVector[3], playStyleOne, playStyleTwo, simSettings[2]);
                            }
                            else {
                                Simulation simulation(simSettings[3], simSettings[1], simSettings[2], simSettings[4], playStyleOne);
                                std::vector<std::vector<long long>> results;
                                auto simResultVector = simulation.Run(1000, simSettings[4]);
                                simResults.setSimulationInfo(simResultVector[0][0], simResultVector[0][1], playStyleOne, "N/A");
                                simResults.setAverageGains(simResultVector[2], simResultVector[3], playStyleOne, "N/A", simSettings[2]);
                            }
                            // After simulation, return to main menu.
                            currState = GameState::SIM_RESULTS;
                        }
                    }
                }
                break;

            case GameState::SIM_RESULTS:
                if (simResults.handleEvent(event, window)) {
                    currState = GameState::MAIN_MENU;
                }
                break;

            case GameState::PLAY:
                playMenu.handleEvent(event, window);
                if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    PlayMenu::Option selected = playMenu.getSelectedOption();
                    if (playMenu.getSelectedOptionPos().contains(static_cast<sf::Vector2f>(mousePos))) {
                        if (selected == PlayMenu::Option::BACK) {
                            currState = GameState::MAIN_MENU;
                        }
                        else if (selected == PlayMenu::Option::START) {
                            // Optionally allow starting the game from the settings screen.
                            gameScreen = std::make_unique<Game>(1200, 800, playMenu.getSelectedStrategy(), playMenu.getGameSettings());
                            currState = GameState::GAME;
                        }
                    }
                }
                break;
            
            case GameState::STATISTICS:
                statisticsMenu.handleEvent(event, window);
                if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                    if (statisticsMenu.getSelectedOption() == StatisticsMenu::Option::BACK) {
                        currState = GameState::MAIN_MENU;
                    }
                }
                break;

            case GameState::GAME:
                if (gameScreen) {
                    gameScreen->handleEvent(event, window);
                    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        Game::Option selected = gameScreen->getSelectedOption();
                        if (gameScreen->getSelectedOptionPos().contains(static_cast<sf::Vector2f>(mousePos))) {
                            if (selected == Game::Option::QUIT) {
                                currState = GameState::MAIN_MENU;
                            }
                        }
                    }
                }
                break;
            }
        }

        // Draw the appropriate screen
        window.clear();
        switch (currState) {
        case GameState::MAIN_MENU:
            window.draw(menuBackground);
            mainMenu.draw(window);
            // Uncomment the following line if you wish to draw the default settings text in MAIN_MENU.
            window.draw(defaultSettingsText);
            break;

        case GameState::SIMULATE:
            window.draw(settingsBackground);
            simulateMenu.draw(window);
            break;

        case GameState::SIM_RESULTS:
            window.draw(menuBackground);
            simResults.draw(window);
            break;

        case GameState::PLAY:
            window.draw(settingsBackground);
            playMenu.draw(window);
            break;

        case GameState::STATISTICS:
            window.draw(settingsBackground);
            statisticsMenu.draw(window);
            break;

        case GameState::GAME:
            window.draw(gameBackground);
            if (gameScreen) {
                gameScreen->draw(window);
            }
            break;
        }
        window.display();
    }

    return 0;
}
