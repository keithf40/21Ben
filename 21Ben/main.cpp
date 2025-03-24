#include <SFML/Graphics.hpp>
#include "MainMenu.h"
#include "SimulateMenu.h"
#include "PlayMenu.h"
#include "Game.h"
#include "Texture.h"

// Enum to manage different game states
enum class GameState { MAIN_MENU, PLAY, SIMULATE, GAME };

int main() {
    // Create main game window
    sf::RenderWindow window(sf::VideoMode({ 1200, 800 }), "Game");

    // Backgrounds for different screens
    sf::RectangleShape menuBackground;
    sf::RectangleShape settingsBackground;
    sf::RectangleShape gameBackground;

    menuBackground.setSize(sf::Vector2f(1200.f, 800.f));
    settingsBackground.setSize(sf::Vector2f(1200.f, 800.f));
    gameBackground.setSize(sf::Vector2f(1200.f, 800.f));

    // Load textures for backgrounds
    Texture textures;
    textures.loadTextures();
    menuBackground.setTexture(&textures.menuBackgroundImage);
    settingsBackground.setTexture(&textures.settingsBackgroundImage);
    gameBackground.setTexture(&textures.gameBackgroundImage);

    // Initialize menus and game screen
    MainMenu mainMenu(1200, 800);
    SimulateMenu simulateMenu(1200, 800);
    PlayMenu playMenu(1200, 800);
    Game gameScreen(1200, 800);

    // Start at the main menu
    GameState currState = GameState::MAIN_MENU;

    // Show the mouse cursor
    window.setMouseCursorVisible(true);

    // Main game loop
    while (window.isOpen()) {
        // Handle events
        while (auto event = window.pollEvent()) {
            // Close the window
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Handle input based on current state
            switch (currState) {
            case GameState::MAIN_MENU:
                mainMenu.handleEvent(*event, window);
                if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonReleased>()) {
                    if (mouseButton->button == sf::Mouse::Button::Left) {
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
                }
                break;

            case GameState::SIMULATE:
                simulateMenu.handleEvent(*event, window);
                if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonReleased>()) {
                    if (mouseButton->button == sf::Mouse::Button::Left) {
                        SimulateMenu::Option selected = simulateMenu.getSelectedOption();
                        if (selected == SimulateMenu::Option::BACK) {
                            currState = GameState::MAIN_MENU;
                        }
                    }
                }
                break;

            case GameState::PLAY:
                playMenu.handleEvent(*event, window);
                if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonReleased>()) {
                    if (mouseButton->button == sf::Mouse::Button::Left) {
                        PlayMenu::Option selected = playMenu.getSelectedOption();
                        if (selected == PlayMenu::Option::BACK) {
                            currState = GameState::MAIN_MENU;
                        }
                        else if (selected == PlayMenu::Option::START) {
                            currState = GameState::GAME;
                        }
                    }
                }
                break;

            case GameState::GAME:
                gameScreen.handleEvent(*event, window);
                if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonReleased>()) {
                    if (mouseButton->button == sf::Mouse::Button::Left) {
                        Game::Option selected = gameScreen.getSelectedOption();
                        if (selected == Game::Option::SKIP) {
                            currState = GameState::MAIN_MENU;
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
            gameScreen.draw(window);
            break;
        }

        // Display everything
        window.display();
    }

    return 0;
}
