#include <SFML/Graphics.hpp>
#include "MainMenu.h"
#include "SimulateMenu.h"
#include "PlayMenu.h"
#include "Game.h"
#include "Texture.h"
#include <memory> // For std::unique_ptr

// Enum to manage different game states
enum class GameState { MAIN_MENU, PLAY, SIMULATE, GAME };

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

    // Use a smart pointer for the game screen and delay its creation
    std::unique_ptr<Game> gameScreen = nullptr;

    // Start at the main menu
    GameState currState = GameState::MAIN_MENU;

    // Show the mouse cursor
    window.setMouseCursorVisible(true);

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
                            currState = GameState::PLAY;
                        }
                        else if (selected == MainMenu::Option::SIMULATE) {
                            currState = GameState::SIMULATE;
                        }
                        else if (selected == MainMenu::Option::EXIT) {
                            window.close();
                        }
                    }
                }
                break;

            case GameState::SIMULATE:
                simulateMenu.handleEvent(event, window);
                if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    SimulateMenu::Option selected = simulateMenu.getSelectedOption();
                    if (playMenu.getSelectedOptionPos().contains(static_cast<sf::Vector2f>(mousePos))) {
                        if (selected == SimulateMenu::Option::BACK) {
                            currState = GameState::MAIN_MENU;
                        }
                        else if (selected == SimulateMenu::Option::START) {
                            // To implement
                        }
                    }
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
                            // Switch to game state and create the game screen at this point
                            currState = GameState::GAME;
                            gameScreen = std::make_unique<Game>(1200, 800);
                        }
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
            break;

        case GameState::SIMULATE:
            window.draw(settingsBackground);
            simulateMenu.draw(window);
            break;

        case GameState::PLAY:
            window.draw(settingsBackground);
            playMenu.draw(window);
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
