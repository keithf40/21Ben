#include "Game.h"
#include <sstream>

Game::Game(float width, float height)
    : player("Player", 200), roundInProgress(false), screenWidth(width), screenHeight(height)
{
    if (!font.loadFromFile("assets/ttfFont.ttf")) {
        std::cerr << "Font not found!" << std::endl;
    }

    // Load all textures (cards and backgrounds)
    textures.loadTextures();

    // Setup the Deal button (shown when round is not active).
    dealButton.setFont(font);
    dealButton.setString("Deal");
    dealButton.setCharacterSize(50);
    dealButton.setFillColor(sf::Color::White);
    dealButton.setPosition(sf::Vector2f(screenWidth - 120.f, 10.f));

    // Setup action buttons (shown during an active round).
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

    // For simplicity, place an initial bet.
    player.placeBet(10);

    // Start the first round.
    startNewRound();
}

void Game::startNewRound() {
    deck.reset();
    dealer.clear();
    player.reset();
    player.placeBet(10);
    roundInProgress = true;
    message = "";

    // Deal initial cards: two to the player, two to the dealer.
    player.hit(deck);
    player.hit(deck);
    dealer.hit(deck); // visible dealer card.
    dealer.hit(deck); // hidden card.
    updateDisplay();

    // If the player's current hand is a natural blackjack, immediately process dealer play.
    if (player.getCurrentHand().isBlackjack()) {
        finishRound();
    }
}

void Game::updateDisplay() {
    // Clear previous sprites.
    playerCardSprites.clear();
    dealerCardSprites.clear();

    // Update player's card sprites for each hand (displayed in separate rows).
    const auto& playerHands = player.getHands();
    float verticalMargin = 20.f;
    for (size_t h = 0; h < playerHands.size(); h++) {
        const auto& cards = playerHands[h].getCards();
        size_t pCount = cards.size();
        float totalWidth = pCount * (cardWidth * scaleFactor) + (pCount - 1) * cardMargin;
        float startX = (screenWidth - totalWidth) / 2.f;
        float y = 500.f + h * ((cardHeight * scaleFactor) + verticalMargin);
        for (size_t i = 0; i < pCount; i++) {
            sf::Sprite sprite;
            sprite.setTexture(getCardTexture(cards[i]));
            sprite.setPosition(startX + i * ((cardWidth * scaleFactor) + cardMargin), y);
            sprite.setScale(scaleFactor, scaleFactor);
            playerCardSprites.push_back(sprite);
        }
    }

    // Update dealer's card sprites.
    const auto& dCards = dealer.getHand().getCards();
    size_t dCount = dCards.size();
    if (roundInProgress && dCount >= 2) {
        float totalDealerWidth = 2 * (cardWidth * scaleFactor) + cardMargin;
        float startXDealer = (screenWidth - totalDealerWidth) / 2.f;
        // Show first dealer card:
        sf::Sprite sprite;
        sprite.setTexture(getCardTexture(dCards[0]));
        sprite.setPosition(startXDealer, 50.f);
        sprite.setScale(scaleFactor, scaleFactor);
        dealerCardSprites.push_back(sprite);
        // Second card hidden:
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
    const auto& hands = player.getHands();
    for (size_t i = 0; i < hands.size(); i++) {
        int handTotal = hands[i].getTotalValue();
        outcomes += "Hand " + std::to_string(i + 1) + ": ";
        if (hands[i].isBlackjack()) {
            outcomes += "Blackjack!";
            // Natural blackjack pays 3:2.
            player.addWinnings(static_cast<int>(player.getCurrentBet() * 2.5));
        }
        else if (handTotal > 21) {
            outcomes += "Busted";
        }
        else if (dealerTotal > 21) {
            outcomes += "Win";
            player.addWinnings(player.getCurrentBet() * 2);
        }
        else if (handTotal > dealerTotal) {
            outcomes += "Win";
            player.addWinnings(player.getCurrentBet() * 2);
        }
        else if (handTotal == dealerTotal) {
            outcomes += "Push";
            player.addWinnings(player.getCurrentBet());
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
            // Mouse clicks on action buttons.
            if (isTextClicked(hitButton, window)) {
                player.hit(deck);
                if (player.isBusted()) {
                    message = "Hand " + std::to_string(player.getHands().size()) + " busted! ";
                    if (!player.advanceHand())
                        finishRound();
                }
                updateDisplay();
            }
            else if (isTextClicked(standButton, window)) {
                if (player.advanceHand()) {
                    if (player.getCurrentHand().getCards().size() < 2) {
                        player.hit(deck);
                    }
                }
                else {
                    finishRound();
                }
                updateDisplay();
            }
            else if (isTextClicked(doubleButton, window)) {
                if (player.getCurrentBet() <= player.getBalance()) {
                    player.placeBet(player.getCurrentBet()); // Deduct additional bet.
                    player.hit(deck);
                    if (!player.advanceHand())
                        finishRound();
                }
                updateDisplay();
            }
            else if (isTextClicked(splitButton, window)) {
                if (player.split()) {
                    if (player.getCurrentHand().getCards().size() < 2) {
                        player.hit(deck);
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
        // Keyboard controls:
        // 'd' to deal if no round is active, 'h' for hit, 's' for stand,
        // 'p' for split, and 'x' for double down.
        switch (event.key.code) {
        case sf::Keyboard::D:
            if (!roundInProgress)
                startNewRound();
            break;
        case sf::Keyboard::H:
            if (roundInProgress) {
                player.hit(deck);
                if (player.isBusted()) {
                    message = "Hand " + std::to_string(player.getHands().size()) + " busted! ";
                    if (!player.advanceHand())
                        finishRound();
                }
                updateDisplay();
            }
            break;
        case sf::Keyboard::S:
            if (roundInProgress) {
                if (player.advanceHand()) {
                    if (player.getCurrentHand().getCards().size() < 2) {
                        player.hit(deck);
                    }
                }
                else {
                    finishRound();
                }
                updateDisplay();
            }
            break;
        case sf::Keyboard::P:
            if (roundInProgress) {
                if (player.split()) {
                    if (player.getCurrentHand().getCards().size() < 2) {
                        player.hit(deck);
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
                if (player.getCurrentBet() <= player.getBalance()) {
                    player.placeBet(player.getCurrentBet()); // Deduct additional bet.
                    player.hit(deck);
                    if (!player.advanceHand())
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
    for (const auto& sprite : dealerCardSprites) {
        window.draw(sprite);
    }
    for (const auto& sprite : playerCardSprites) {
        window.draw(sprite);
    }
    window.draw(messageText);
}

Game::Option Game::getSelectedOption() const {
    return Option::NONE;
}
