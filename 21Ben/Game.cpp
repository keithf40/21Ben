#include "Game.h"
#include <sstream>

Game::Game(float width, float height)
    : player("Player", 200), roundInProgress(false)
{
    if (!font.loadFromFile("assets/ttfFont.ttf")) {
        std::cerr << "Font not found!" << std::endl;
    }

    // Create "Skip" button.
    sf::Text skipButton;
    skipButton.setFont(font);
    skipButton.setString("Skip");
    skipButton.setCharacterSize(50);
    skipButton.setFillColor(sf::Color::White);
    skipButton.setPosition(sf::Vector2f(width - 120.f, 10.f)); // Adjust as needed.
    menuOptions.push_back(skipButton);

    // Setup action buttons for Hit and Stand.
    hitButton.setFont(font);
    hitButton.setString("Hit");
    hitButton.setCharacterSize(40);
    hitButton.setFillColor(sf::Color::White);
    hitButton.setPosition(sf::Vector2f(50.f, height - 120.f));

    standButton.setFont(font);
    standButton.setString("Stand");
    standButton.setCharacterSize(40);
    standButton.setFillColor(sf::Color::White);
    standButton.setPosition(sf::Vector2f(200.f, height - 120.f));

    // Setup display texts for player's hand, dealer's hand, and message.
    playerHandText.setFont(font);
    playerHandText.setCharacterSize(30);
    playerHandText.setFillColor(sf::Color::White);
    playerHandText.setPosition(50.f, height - 200.f);

    dealerHandText.setFont(font);
    dealerHandText.setCharacterSize(30);
    dealerHandText.setFillColor(sf::Color::White);
    dealerHandText.setPosition(50.f, 50.f);

    messageText.setFont(font);
    messageText.setCharacterSize(40);
    messageText.setFillColor(sf::Color::Yellow);
    messageText.setPosition(width / 2.f - 200.f, height / 2.f - 50.f);

    // Start the first round.
    startNewRound();
}

void Game::startNewRound() {
    // Reset deck, dealer, and player.
    deck.reset();
    dealer.clear();
    player.reset();
    roundInProgress = true;
    message = "Click Hit to draw a card, or Stand to finish your turn.";

    // Deal initial cards: two to the player, two to the dealer.
    player.hit(deck);
    player.hit(deck);
    dealer.hit(deck); // visible dealer card.
    dealer.hit(deck); // hidden card.

    updateDisplayText();
}

void Game::updateDisplayText() {
    // Update player's hand display.
    std::ostringstream pStream;
    pStream << player.toString();
    playerHandText.setString(pStream.str());

    // Update dealer's hand display.
    std::ostringstream dStream;
    if (roundInProgress) {
        // Show only the first dealer card.
        std::string fullDealerHand = dealer.getHand().toString();
        size_t pos = fullDealerHand.find(",");
        if (pos != std::string::npos) {
            dStream << "Dealer's Hand: " << fullDealerHand.substr(0, pos) << ", [Hidden]";
        }
        else {
            dStream << "Dealer's Hand: " << fullDealerHand;
        }
    }
    else {
        dStream << "Dealer's Hand: " << dealer.getHand().toString();
    }
    dealerHandText.setString(dStream.str());

    // Update message text.
    messageText.setString(message);
}

void Game::finishRound() {
    // Process dealer turn.
    int dealerTotal = dealer.dealerTurn(deck);
    int playerTotal = player.getHand().getTotalValue();

    if (player.isBusted()) {
        message = "You busted! Click Skip to restart.";
    }
    else if (dealerTotal > 21) {
        message = "Dealer busted! You win! Click Skip to restart.";
        player.addWinnings(player.getCurrentBet() * 2);
    }
    else if (playerTotal > dealerTotal) {
        message = "You win! Click Skip to restart.";
        player.addWinnings(player.getCurrentBet() * 2);
    }
    else if (playerTotal == dealerTotal) {
        message = "Push! Click Skip to restart.";
        player.addWinnings(player.getCurrentBet()); // return bet.
    }
    else {
        message = "You lose! Click Skip to restart.";
    }
    roundInProgress = false;
    updateDisplayText();
}

bool Game::isTextClicked(const sf::Text& text, sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return text.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

void Game::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    // Handle menu option (Skip) with mouse movement and click.
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        for (std::size_t i = 0; i < menuOptions.size(); ++i) {
            if (menuOptions[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                menuOptions[i].setFillColor(sf::Color::Yellow);
                selectedMenuIndex = i;
            }
            else {
                menuOptions[i].setFillColor(sf::Color::White);
            }
        }
        // Highlight action buttons.
        if (isTextClicked(hitButton, window)) {
            hitButton.setFillColor(sf::Color::Yellow);
            hitHovered = true;
        }
        else {
            hitButton.setFillColor(sf::Color::White);
            hitHovered = false;
        }
        if (isTextClicked(standButton, window)) {
            standButton.setFillColor(sf::Color::Yellow);
            standHovered = true;
        }
        else {
            standButton.setFillColor(sf::Color::White);
            standHovered = false;
        }
    }

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        // Check Skip button.
        if (!menuOptions.empty() && isTextClicked(menuOptions[selectedMenuIndex], window)) {
            // Restart the round if Skip is clicked.
            startNewRound();
        }
        // Handle Hit button.
        if (isTextClicked(hitButton, window) && roundInProgress) {
            player.hit(deck);
            if (player.isBusted()) {
                message = "You busted! Click Skip to restart.";
                roundInProgress = false;
            }
            updateDisplayText();
        }
        // Handle Stand button.
        if (isTextClicked(standButton, window) && roundInProgress) {
            finishRound();
        }
    }
}

void Game::draw(sf::RenderWindow& window) {
    // Draw menu options (Skip button).
    for (const auto& option : menuOptions) {
        window.draw(option);
    }
    // Draw action buttons.
    window.draw(hitButton);
    window.draw(standButton);
    // Draw texts: player's hand, dealer's hand, and messages.
    window.draw(playerHandText);
    window.draw(dealerHandText);
    window.draw(messageText);
}

Game::Option Game::getSelectedOption() const {
    if (selectedMenuIndex == 0) {
        return Option::SKIP;
    }
    return Option::NONE;
}
