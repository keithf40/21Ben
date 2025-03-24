#include "Game.h"
#include <sstream>

// Constructor – initializes screen dimensions, players, and positions.
Game::Game(float width, float height)
    : roundInProgress(false), screenWidth(width), screenHeight(height)
{
    if (!font.loadFromFile("assets/ttfFont.ttf")) {
        std::cerr << "Font not found!" << std::endl;
    }

    // Load textures.
    textures.loadTextures();

    // Setup UI buttons.
    dealButton.setFont(font);
    dealButton.setString("Deal");
    dealButton.setCharacterSize(50);
    dealButton.setFillColor(sf::Color::White);
    dealButton.setPosition(sf::Vector2f(screenWidth - 120.f, 10.f));

    hitButton.setFont(font);
    hitButton.setString("Hit");
    hitButton.setCharacterSize(40);
    hitButton.setFillColor(sf::Color::White);
    hitButton.setPosition(sf::Vector2f(50.f, screenHeight - 120.f));

    standButton.setFont(font);
    standButton.setString("Stand");
    standButton.setCharacterSize(40);
    standButton.setFillColor(sf::Color::White);
    standButton.setPosition(sf::Vector2f(200.f, screenHeight - 120.f));

    doubleButton.setFont(font);
    doubleButton.setString("Double");
    doubleButton.setCharacterSize(40);
    doubleButton.setFillColor(sf::Color::White);
    doubleButton.setPosition(sf::Vector2f(350.f, screenHeight - 120.f));

    splitButton.setFont(font);
    splitButton.setString("Split");
    splitButton.setCharacterSize(40);
    splitButton.setFillColor(sf::Color::White);
    splitButton.setPosition(sf::Vector2f(520.f, screenHeight - 120.f));

    // Setup message text.
    messageText.setFont(font);
    messageText.setCharacterSize(40);
    messageText.setFillColor(sf::Color::Yellow);
    messageText.setPosition(screenWidth / 2.f - 200.f, screenHeight / 2.f - 50.f);

    // Initialize 7 players.
    // We want human at index 3.
    players.push_back(Player("Bot 1", 200));
    players.push_back(Player("Bot 2", 200));
    players.push_back(Player("Bot 3", 200));
    players.push_back(Player("Human", 200));
    players.push_back(Player("Bot 4", 200));
    players.push_back(Player("Bot 5", 200));
    players.push_back(Player("Bot 6", 200));

    // Set up positions for each player's hand.
    // Left side: indices 0,1,2.
    // Center (human): index 3.
    // Right side: indices 4,5,6.
    playerPositions.resize(7);
    playerPositions[0] = sf::Vector2f(50.f, 100.f);
    playerPositions[1] = sf::Vector2f(50.f, 250.f);
    playerPositions[2] = sf::Vector2f(50.f, 400.f);
    playerPositions[3] = sf::Vector2f(screenWidth / 2.f - 100.f, 500.f);  // Human in center.
    playerPositions[4] = sf::Vector2f(screenWidth - 200.f, 400.f);
    playerPositions[5] = sf::Vector2f(screenWidth - 200.f, 250.f);
    playerPositions[6] = sf::Vector2f(screenWidth - 200.f, 100.f);

    // Reserve space for players’ card sprites.
    playersCardSprites.resize(players.size());

    // For simplicity, set an initial bet for each player.
    for (auto& player : players) {
        player.placeBet(10);
    }

    // Start the first round.
    startNewRound();
}

void Game::startNewRound() {
    deck.reset();
    dealer.clear();

    // Reset each player's state.
    for (auto& player : players) {
        player.reset();
        player.placeBet(10);
    }
    roundInProgress = true;
    message = "";

    // Deal two cards to each player.
    for (auto& player : players) {
        player.hit(deck);
        player.hit(deck);
    }
    // Deal two cards to dealer.
    dealer.hit(deck);
    dealer.hit(deck);

    updateDisplay();

    // Check for natural blackjack in the human player's hand.
    if (players[humanIndex].getCurrentHand().isBlackjack()) {
        finishRound();
        return;
    }

    // Simulate moves for bot players automatically.
    simulateBotMoves();
}

void Game::simulateBotMoves() {
    // For each bot (all players except human), simulate optimal play.
    // Use the dealer's upcard (first card) for decision-making.
    Card dealerUp = dealer.getHand().getCards()[0];
    for (int i = 0; i < players.size(); i++) {
        if (i == humanIndex)
            continue;
        // For each bot, while the bot's current hand is not busted and not standing,
        // use OptimalPlay to decide.
        // We'll assume that bot actions are simulated instantly.
        bool botTurn = true;
        while (botTurn && !players[i].isBusted()) {
            char move = optimalPlay.getMove(players[i].getCurrentHand(), dealerUp);
            // Moves: 'H' = hit, 'S' = stand, 'D' = double down, 'P' = split.
            if (move == 'H') {
                players[i].hit(deck);
                if (players[i].isBusted())
                    botTurn = false;
            }
            else if (move == 'S') {
                botTurn = false;
            }
            else if (move == 'D') {
                if (players[i].getCurrentBet() <= players[i].getBalance()) {
                    players[i].placeBet(players[i].getCurrentBet());
                    players[i].hit(deck);
                }
                botTurn = false;
            }
            else if (move == 'P') {
                if (players[i].split()) {
                    if (players[i].getCurrentHand().getCards().size() < 2)
                        players[i].hit(deck);
                }
                else {
                    // If cannot split, default to hit.
                    players[i].hit(deck);
                }
            }
            else {
                // Default to stand.
                botTurn = false;
            }
        }
    }
    updateDisplay();
}

void Game::updateDisplay() {
    // Clear previous sprites.
    for (auto& vec : playersCardSprites)
        vec.clear();
    dealerCardSprites.clear();

    // Update each player's card sprites.
    for (size_t i = 0; i < players.size(); i++) {
        const auto& hand = players[i].getCurrentHand().getCards();
        size_t pCount = hand.size();
        // We'll draw each player's hand starting at their designated position.
        sf::Vector2f pos = playerPositions[i];
        // Calculate total width.
        float totalWidth = pCount * (cardWidth * scaleFactor) + (pCount - 1) * cardMargin;
        float startX = pos.x; // For simplicity, use pos.x directly.
        // (You could center relative to a region if desired.)
        for (size_t j = 0; j < pCount; j++) {
            sf::Sprite sprite;
            sprite.setTexture(getCardTexture(hand[j]));
            sprite.setPosition(startX + j * ((cardWidth * scaleFactor) + cardMargin), pos.y);
            sprite.setScale(scaleFactor, scaleFactor);
            playersCardSprites[i].push_back(sprite);
        }
    }

    // Update dealer's card sprites.
    const auto& dCards = dealer.getHand().getCards();
    size_t dCount = dCards.size();
    if (roundInProgress && dCount >= 2) {
        // Show only the first dealer card and hide the second.
        float totalDealerWidth = 2 * (cardWidth * scaleFactor) + cardMargin;
        float startXDealer = (screenWidth - totalDealerWidth) / 2.f;
        sf::Sprite sprite;
        sprite.setTexture(getCardTexture(dCards[0]));
        sprite.setPosition(startXDealer, 50.f);
        sprite.setScale(scaleFactor, scaleFactor);
        dealerCardSprites.push_back(sprite);
        sf::Sprite hiddenSprite;
        hiddenSprite.setTexture(textures.backOfCard);
        hiddenSprite.setPosition(startXDealer + (cardWidth * scaleFactor) + cardMargin, 50.f);
        hiddenSprite.setScale(scaleFactor, scaleFactor);
        dealerCardSprites.push_back(hiddenSprite);
    }
    else {
        float totalDealerWidth = dCount * (cardWidth * scaleFactor) + (dCount - 1) * cardMargin;
        float startXDealer = (screenWidth - totalDealerWidth) / 2.f;
        for (size_t i = 0; i < dCount; i++) {
            sf::Sprite sprite;
            sprite.setTexture(getCardTexture(dCards[i]));
            sprite.setPosition(startXDealer + i * ((cardWidth * scaleFactor) + cardMargin), 50.f);
            sprite.setScale(scaleFactor, scaleFactor);
            dealerCardSprites.push_back(sprite);
        }
    }

    // Update message text.
    messageText.setString(message);
}

void Game::finishRound() {
    int dealerTotal = dealer.dealerTurn(deck);
    std::string outcomes;
    for (size_t i = 0; i < players.size(); i++) {
        int handTotal = players[i].getCurrentHand().getTotalValue();
        outcomes += players[i].getName() + ": ";
        if (players[i].getCurrentHand().isBlackjack()) {
            outcomes += "Blackjack!";
            players[i].addWinnings(static_cast<int>(players[i].getCurrentBet() * 2.5));
        }
        else if (handTotal > 21) {
            outcomes += "Busted";
        }
        else if (dealerTotal > 21) {
            outcomes += "Win";
            players[i].addWinnings(players[i].getCurrentBet() * 2);
        }
        else if (handTotal > dealerTotal) {
            outcomes += "Win";
            players[i].addWinnings(players[i].getCurrentBet() * 2);
        }
        else if (handTotal == dealerTotal) {
            outcomes += "Push";
            players[i].addWinnings(players[i].getCurrentBet());
        }
        else {
            outcomes += "Lose";
        }
        outcomes += "\n";
    }
    message = outcomes + "Click Deal to restart.";
    roundInProgress = false;
    updateDisplay();
}

bool Game::isTextClicked(const sf::Text& text, sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return text.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

sf::Texture& Game::getCardTexture(const Card& card) {
    if (card.getSuit() == Suit::Hearts) {
        switch (card.getRank()) {
        case Rank::Two:   return textures.hearts2Texture;
        case Rank::Three: return textures.hearts3Texture;
        case Rank::Four:  return textures.hearts4Texture;
        case Rank::Five:  return textures.hearts5Texture;
        case Rank::Six:   return textures.hearts6Texture;
        case Rank::Seven: return textures.hearts7Texture;
        case Rank::Eight: return textures.hearts8Texture;
        case Rank::Nine:  return textures.hearts9Texture;
        case Rank::Ten:   return textures.hearts10Texture;
        case Rank::Jack:  return textures.heartsjackTexture;
        case Rank::Queen: return textures.heartsqueenTexture;
        case Rank::King:  return textures.heartskingTexture;
        case Rank::Ace:   return textures.heartsaceTexture;
        }
    }
    else if (card.getSuit() == Suit::Diamonds) {
        switch (card.getRank()) {
        case Rank::Two:   return textures.diamonds2Texture;
        case Rank::Three: return textures.diamonds3Texture;
        case Rank::Four:  return textures.diamonds4Texture;
        case Rank::Five:  return textures.diamonds5Texture;
        case Rank::Six:   return textures.diamonds6Texture;
        case Rank::Seven: return textures.diamonds7Texture;
        case Rank::Eight: return textures.diamonds8Texture;
        case Rank::Nine:  return textures.diamonds9Texture;
        case Rank::Ten:   return textures.diamonds10Texture;
        case Rank::Jack:  return textures.diamondsjackTexture;
        case Rank::Queen: return textures.diamondsqueenTexture;
        case Rank::King:  return textures.diamondskingTexture;
        case Rank::Ace:   return textures.diamondsaceTexture;
        }
    }
    else if (card.getSuit() == Suit::Spades) {
        switch (card.getRank()) {
        case Rank::Two:   return textures.spades2Texture;
        case Rank::Three: return textures.spades3Texture;
        case Rank::Four:  return textures.spades4Texture;
        case Rank::Five:  return textures.spades5Texture;
        case Rank::Six:   return textures.spades6Texture;
        case Rank::Seven: return textures.spades7Texture;
        case Rank::Eight: return textures.spades8Texture;
        case Rank::Nine:  return textures.spades9Texture;
        case Rank::Ten:   return textures.spades10Texture;
        case Rank::Jack:  return textures.spadesjackTexture;
        case Rank::Queen: return textures.spadesqueenTexture;
        case Rank::King:  return textures.spadeskingTexture;
        case Rank::Ace:   return textures.spadesaceTexture;
        }
    }
    else if (card.getSuit() == Suit::Clubs) {
        switch (card.getRank()) {
        case Rank::Two:   return textures.clubs2Texture;
        case Rank::Three: return textures.clubs3Texture;
        case Rank::Four:  return textures.clubs4Texture;
        case Rank::Five:  return textures.clubs5Texture;
        case Rank::Six:   return textures.clubs6Texture;
        case Rank::Seven: return textures.clubs7Texture;
        case Rank::Eight: return textures.clubs8Texture;
        case Rank::Nine:  return textures.clubs9Texture;
        case Rank::Ten:   return textures.clubs10Texture;
        case Rank::Jack:  return textures.clubsjackTexture;
        case Rank::Queen: return textures.clubsqueenTexture;
        case Rank::King:  return textures.clubskingTexture;
        case Rank::Ace:   return textures.clubsaceTexture;
        }
    }
    return textures.backOfCard;
}

void Game::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    // Handle both mouse and keyboard events.
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (!roundInProgress) {
            if (isTextClicked(dealButton, window)) {
                startNewRound();
            }
        }
        else {
            // Only the human player's actions are processed by input.
            if (isTextClicked(hitButton, window)) {
                players[humanIndex].hit(deck);
                if (players[humanIndex].isBusted()) {
                    message = "You busted!";
                    finishRound();
                }
                updateDisplay();
            }
            else if (isTextClicked(standButton, window)) {
                // Human stands.
                finishRound();
                updateDisplay();
            }
            else if (isTextClicked(doubleButton, window)) {
                if (players[humanIndex].getCurrentBet() <= players[humanIndex].getBalance()) {
                    players[humanIndex].placeBet(players[humanIndex].getCurrentBet());
                    players[humanIndex].hit(deck);
                    finishRound();
                }
                updateDisplay();
            }
            else if (isTextClicked(splitButton, window)) {
                if (players[humanIndex].split()) {
                    if (players[humanIndex].getCurrentHand().getCards().size() < 2) {
                        players[humanIndex].hit(deck);
                    }
                    updateDisplay();
                }
                else {
                    message = "Cannot split this hand.";
                    updateDisplay();
                }
            }
        }
    }
    else if (event.type == sf::Event::KeyReleased) {
        // Keyboard controls for human player.
        // d: deal (if no round active), h: hit, s: stand, p: split, x: double down.
        switch (event.key.code) {
        case sf::Keyboard::D:
            if (!roundInProgress)
                startNewRound();
            break;
        case sf::Keyboard::H:
            if (roundInProgress) {
                players[humanIndex].hit(deck);
                if (players[humanIndex].isBusted()) {
                    message = "You busted!";
                    finishRound();
                }
                updateDisplay();
            }
            break;
        case sf::Keyboard::S:
            if (roundInProgress) {
                finishRound();
                updateDisplay();
            }
            break;
        case sf::Keyboard::P:
            if (roundInProgress) {
                if (players[humanIndex].split()) {
                    if (players[humanIndex].getCurrentHand().getCards().size() < 2) {
                        players[humanIndex].hit(deck);
                    }
                    updateDisplay();
                }
                else {
                    message = "Cannot split this hand.";
                    updateDisplay();
                }
            }
            break;
        case sf::Keyboard::X:
            if (roundInProgress) {
                if (players[humanIndex].getCurrentBet() <= players[humanIndex].getBalance()) {
                    players[humanIndex].placeBet(players[humanIndex].getCurrentBet());
                    players[humanIndex].hit(deck);
                    finishRound();
                }
                updateDisplay();
            }
            break;
        default:
            break;
        }
    }
}

void Game::draw(sf::RenderWindow& window) {
    if (!roundInProgress) {
        window.draw(dealButton);
    }
    else {
        window.draw(hitButton);
        window.draw(standButton);
        window.draw(doubleButton);
        window.draw(splitButton);
    }
    for (const auto& vec : playersCardSprites) {
        for (const auto& sprite : vec) {
            window.draw(sprite);
        }
    }
    for (const auto& sprite : dealerCardSprites) {
        window.draw(sprite);
    }
    window.draw(messageText);
}

Game::Option Game::getSelectedOption() const {
    return Option::NONE;
}
