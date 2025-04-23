#include "Game.h"
#include <sstream>
#include <algorithm>

Game::Game(float width, float height, std::string countingMethod, std::vector<int> gameSettings)
    : roundInProgress(false), screenWidth(width), screenHeight(height), strategy(countingMethod)
{
    if (!font.loadFromFile("assets/ttfFont.ttf")) {
        std::cerr << "Font not found!" << std::endl;
    }
    if (gameSettings[0]) {
        minBet = gameSettings[1];
        deck = Deck(gameSettings[3]);
        startingMoney = gameSettings[2];
        // do player position
    }
    counter.setStrategy(countingMethod);
    // Load all textures.
    textures.loadTextures();

    // Setup the Deal button.
    dealButton.setFont(font);
    dealButton.setString("Deal");
    dealButton.setCharacterSize(50);
    dealButton.setFillColor(sf::Color::White);
    dealButton.setPosition(sf::Vector2f(screenWidth - 120.f, 10.f));

    // Setup human action buttons.
    hitButton.setFont(font);
    hitButton.setString("Hit");
    hitButton.setCharacterSize(40);
    hitButton.setFillColor(sf::Color::White);

    standButton.setFont(font);
    standButton.setString("Stand");
    standButton.setCharacterSize(40);
    standButton.setFillColor(sf::Color::White);

    doubleButton.setFont(font);
    doubleButton.setString("Double");
    doubleButton.setCharacterSize(40);
    doubleButton.setFillColor(sf::Color::White);

    splitButton.setFont(font);
    splitButton.setString("Split");
    splitButton.setCharacterSize(40);
    splitButton.setFillColor(sf::Color::White);

    quitButton.setFont(font);
    quitButton.setString("Quit");
    quitButton.setCharacterSize(40);
    quitButton.setFillColor(sf::Color::White);
    quitButton.setPosition(sf::Vector2f(10.f, height / 1.1f));

    placeBetButton.setFont(font);
    placeBetButton.setString("Place Bet");
    placeBetButton.setCharacterSize(50);
    placeBetButton.setFillColor(sf::Color::White);
    placeBetButton.setPosition(sf::Vector2f(screenWidth / 2.f - 100.f, screenHeight / 2.f - 100.f));

    // Setup message text.
    messageText.setFont(font);
    messageText.setCharacterSize(40);
    messageText.setFillColor(sf::Color::Yellow);
    messageText.setPosition(screenWidth / 2.f - 200.f, screenHeight / 2.f - 50.f);

    // Initialize 5 players: 4 bots and 1 human.
    // Bots are indices 0-3; human is at index 4.
    players.push_back(Player("Bot 1", startingMoney));
    players.push_back(Player("Bot 2", startingMoney));
    players.push_back(Player("Bot 3", startingMoney));
    players.push_back(Player("Bot 4", startingMoney));
    players.push_back(Player("Human", startingMoney));

    // Reserve space for players' card sprites.
    playersCardSprites.resize(players.size());

    // Setup positions for each player's hand.
    // Place bots in the four corners and human at bottom center.
    playerPositions.resize(5);
    playerPositions[0] = sf::Vector2f(100.f, 80.f);                       // Bot 1: Top left.
    playerPositions[1] = sf::Vector2f(screenWidth - 200.f, 80.f);           // Bot 2: Top right.
    playerPositions[2] = sf::Vector2f(100.f, screenHeight - 250.f);         // Bot 3: Bottom left.
    playerPositions[3] = sf::Vector2f(screenWidth - 200.f, screenHeight - 250.f); // Bot 4: Bottom right.
    // Human at bottom center. X will be recalculated in updateDisplay() so the hand is centered.
    playerPositions[4] = sf::Vector2f(0.f, screenHeight - 250.f);

    //// Start round and set turn order.
    //startNewRound();

    // Position the action buttons under the human's hand.
    // Human hand y is at screenHeight - 250, so we place buttons slightly below that.
    float actionX = (screenWidth / 2.f) - 160.f;
    float actionY = screenHeight - 50.f;  // moved up a tad so they don't cover the player cards
    hitButton.setPosition(actionX, actionY);
    standButton.setPosition(actionX + 120.f, actionY);
    doubleButton.setPosition(actionX + 240.f, actionY);
    splitButton.setPosition(actionX + 360.f, actionY);

    // initial bet defaults to minBet
    currentBetAmount = minBet;

    sliderTrack.setSize({ 250.f, 5.f });
    sliderTrack.setFillColor(sf::Color::White);
    float trackY = placeBetButton.getPosition().y
        + placeBetButton.getCharacterSize()
        + 60.f;   // <- increased from 20 to 60
    sliderTrack.setPosition(placeBetButton.getPosition().x, trackY);
    sliderKnob.setPosition(
        sliderTrack.getPosition().x,
        trackY + sliderTrack.getSize().y / 2.f
    );
    sliderLabel.setPosition(
        sliderTrack.getPosition().x,
        trackY - 30.f  // stays 30px above the track
    );


    sliderKnob.setSize({ 15.f, 25.f });
    sliderKnob.setFillColor(sf::Color::Yellow);
    // origin at center so we can clamp by knob center
    sliderKnob.setOrigin(sliderKnob.getSize() / 2.f);
    // start at leftmost
    sliderKnob.setPosition(
        sliderTrack.getPosition().x,
        sliderTrack.getPosition().y + sliderTrack.getSize().y / 2.f
    );
    sliderLabel.setFont(font);
    sliderLabel.setCharacterSize(30);
    sliderLabel.setFillColor(sf::Color::White);
    sliderLabel.setPosition(
        sliderTrack.getPosition().x,
        sliderTrack.getPosition().y - 50.f
    );
    sliderLabel.setString("Bet: " + std::to_string(currentBetAmount));

    remainingMoneyText.setFont(font);
    remainingMoneyText.setCharacterSize(30);
    remainingMoneyText.setFillColor(sf::Color::White);
    // put it just below the slider:
    remainingMoneyText.setPosition(
        sliderTrack.getPosition().x,
        sliderTrack.getPosition().y + sliderTrack.getSize().y + 10.f
    );
    // initial value
    int startBal = players[humanIndex].getBalance();
    remainingMoneyText.setString("Remaining: " + std::to_string(startBal - currentBetAmount));

    turnsUntilPlayText.setFont(font);
    turnsUntilPlayText.setCharacterSize(40);
    turnsUntilPlayText.setFillColor(sf::Color::White);
    turnsUntilPlayText.setPosition(screenWidth / 2.5, screenHeight / 2 - 20);
    turnsUntilPlayText.setString("");

    countText.setFont(font);
    countText.setCharacterSize(24);
    countText.setFillColor(sf::Color::White);
    countText.setPosition(10.f, 10.f);
    countText.setString("");
}

void Game::startNewRound() {
    
    dealer.clear();
    for (size_t i = 0; i < players.size(); ++i) {
        players[i].reset();
        if (i == humanIndex) {
            players[i].placeBet(currentBetAmount);
        }
        else {
            players[i].placeBet(
                counter.getBet(deck.getDecksRemaining(),
                    players[i].getBalance(),
                    minBet, false));
        }
    }
    roundInProgress = true;
    message = "";
    currentPlayerTurn = 0;  // Start with the first player.
    botClock.restart();

    // Deal two cards to each player.
    for (auto& p : players) {
        p.hit(deck, counter);
        p.hit(deck, counter);
    }
    // Deal two cards to dealer.
    dealer.hit(deck, counter, false);
    dealer.hit(deck, counter, true);
    updateDisplay();

    // Check if human (player index 4) has blackjack.
    if (players[humanIndex].getCurrentHand().isBlackjack()) {
        finishRound();
        return;
    }
}

void Game::updateBotMoves() {
    if (!roundInProgress) return;

    // Process bot moves only if it's currently a bot's turn.
    if (currentPlayerTurn < players.size() && currentPlayerTurn != humanIndex) {
        // Change delay to 2 seconds.
        if (botClock.getElapsedTime().asSeconds() >= 2.0f) {
            Card dealerUp = dealer.getHand().getCards()[0];
            char move = optimalPlay.getMove(players[currentPlayerTurn].getCurrentHand(), dealerUp);
            if (move == 'H') {
                players[currentPlayerTurn].hit(deck, counter);
                if (players[currentPlayerTurn].isBusted()) {
                    if (!players[currentPlayerTurn].advanceHand())
                        currentPlayerTurn++;
                }
            }
            else if (move == 'S') {
                if (!players[currentPlayerTurn].advanceHand())
                    currentPlayerTurn++;
            }
            else if (move == 'D') {
                if (players[currentPlayerTurn].getCurrentBet() <= players[currentPlayerTurn].getBalance()) {
                    players[currentPlayerTurn].placeBet(players[currentPlayerTurn].getCurrentBet());
                    players[currentPlayerTurn].hit(deck, counter);
                }
                if (!players[currentPlayerTurn].advanceHand())
                    currentPlayerTurn++;
            }
            else if (move == 'Y') {
                players[currentPlayerTurn].split();
            }
            else {
                if (!players[currentPlayerTurn].advanceHand())
                    currentPlayerTurn++;
            }
            botClock.restart();
            updateDisplay();
        }
    }
}

void Game::updateDisplay() {
    // Clear previous sprites.
    for (auto& vec : playersCardSprites)
        vec.clear();
    dealerCardSprites.clear();

    // Update each player's card sprites.
    for (size_t i = 0; i < players.size(); i++) {
        const auto& hand = players[i].getCurrentHand().getCards();
        size_t count = hand.size();
        sf::Vector2f pos = playerPositions[i];
        float totalWidth = count * (cardWidth * scaleFactor) + (count - 1) * cardMargin;
        float startX;
        // For the human player (index 4), recalc x so that the hand is centered.
        if (i == humanIndex)
            startX = (screenWidth - totalWidth) / 2.f;
        else
            startX = pos.x;
        for (size_t j = 0; j < count; j++) {
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
        // Dealer hand is drawn at y = 20.
        float totalDealerWidth = 2 * (cardWidth * scaleFactor) + cardMargin;
        float startXDealer = (screenWidth - totalDealerWidth) / 2.f;
        sf::Sprite sprite;
        sprite.setTexture(getCardTexture(dCards[0]));
        sprite.setPosition(startXDealer, 20.f);
        sprite.setScale(scaleFactor, scaleFactor);
        dealerCardSprites.push_back(sprite);
        sf::Sprite hiddenSprite;
        hiddenSprite.setTexture(textures.backOfCard);
        hiddenSprite.setPosition(startXDealer + (cardWidth * scaleFactor) + cardMargin, 20.f);
        hiddenSprite.setScale(scaleFactor, scaleFactor);
        dealerCardSprites.push_back(hiddenSprite);
    }
    else {
        float totalDealerWidth = dCount * (cardWidth * scaleFactor) + (dCount - 1) * cardMargin;
        float startXDealer = (screenWidth - totalDealerWidth) / 2.f;
        for (size_t i = 0; i < dCount; i++) {
            sf::Sprite sprite;
            sprite.setTexture(getCardTexture(dCards[i]));
            sprite.setPosition(startXDealer + i * ((cardWidth * scaleFactor) + cardMargin), 20.f);
            sprite.setScale(scaleFactor, scaleFactor);
            dealerCardSprites.push_back(sprite);
        }
    }

    // Update message text.
    messageText.setString(message);

    if (strategy != "None") {
        countText.setString("Count: " + std::to_string(counter.getCount()));
    }
    else {
        countText.setString("");
    }
}

void Game::finishRound() {
    int dealerTotal = dealer.dealerTurn(deck, counter);
    // Only display the outcome for the human player.
    int humanTotal = players[humanIndex].getCurrentHand().getTotalValue();
    std::string outcome = "Human: ";
    bool isWin = false;
    bool isDraw = false;
    int totalMoneyEarned = players[humanIndex].totalWinnings(dealerTotal, dealer.checkBlackjack());
    players[humanIndex].addWinnings(totalMoneyEarned);

    if (players[humanIndex].getCurrentHand().isBlackjack()) {
        outcome += "Blackjack!";
        isWin = true;
    }
    else if (humanTotal > 21) {
        outcome += "Busted";
        isWin = false;
    }
    else if (dealerTotal > 21) {
        outcome += "Win";
        isWin = true;
    }
    else if (humanTotal > dealerTotal) {
        outcome += "Win";
        isWin = true;
    }
    else if (humanTotal == dealerTotal) {
        outcome += "Push";
        isDraw = true;
    }
    else {
        outcome += "Lose";
        isWin = false;
    }

    message = outcome;
    roundInProgress = false;
    //adds the face down dealer card to the running count
    Hand dealerHand = dealer.getHand();
    counter.modifyCount(dealerHand.getCards()[1]);

    // Record the game statistics
    recordGameStats(isWin, isDraw);

    updateDisplay();
    roundInProgress = false;
    bettingPhase = false;
    readyToBet = false;
    if (deck.shuffleReady()) {
        deck.reset();
        counter.resetCount();
    }
}

void Game::recordGameStats(bool isWin, bool isDraw) {
    // Get the human player's bet and balance
    int currentBet = players[humanIndex].getCurrentBet();

    // Calculate chips won or lost
    int chipsWon = 0;
    int chipsLost = 0;

    if (isWin) {
        // On a win, player gets their bet back plus winnings
        if (players[humanIndex].getCurrentHand().isBlackjack()) {
            // Blackjack pays 3:2
            chipsWon = currentBet * 1.5;
        }
        else {
            chipsWon = currentBet;
        }
    }
    else if (!isDraw) {
        // On a loss, player loses their bet
        chipsLost = currentBet;
    }
    // On a draw (push), neither won nor lost

    // Record the outcome in the stats tracker
    StatsTracker::getInstance().recordGamePlayed(isWin, isDraw, chipsWon, chipsLost, currentBet, strategy);
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
    // Process mouse and keyboard events.
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mf(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        if (isDraggingSlider) {
            float newX = static_cast<float>(mousePos.x);
            float leftBound = sliderTrack.getPosition().x;
            float rightBound = leftBound + sliderTrack.getSize().x;
            // clamp to track ends
            newX = (newX < leftBound) ? leftBound : (newX > rightBound ? rightBound : newX);
            sliderKnob.setPosition(
                newX - sliderKnob.getSize().x / 2.f,
                sliderTrack.getPosition().y + sliderTrack.getSize().y / 2.f
            );

            float ratio = (newX - leftBound) / sliderTrack.getSize().x;
            int maxBet = players[humanIndex].getBalance();

            // map to [minBet, maxBet], then quantize to 5
            double raw = minBet + ratio * (maxBet - minBet);
            int quantized = minBet + static_cast<int>(std::round((raw - minBet) / 5.0)) * 5;
            if (quantized < minBet) quantized = minBet;
            if (quantized > maxBet) quantized = maxBet;

            currentBetAmount = quantized;
            sliderLabel.setString("Bet: " + std::to_string(currentBetAmount));
            int rem = players[humanIndex].getBalance() - currentBetAmount;
            remainingMoneyText.setString("Remaining: " + std::to_string(rem));
        }


        // Highlight Quit button
        if (quitButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            quitButton.setFillColor(sf::Color::Yellow);
        }
        else {
            quitButton.setFillColor(sf::Color::White);
        }

        if (!roundInProgress && readyToBet && bettingPhase) {
            if (placeBetButton.getGlobalBounds().contains(mf))
                placeBetButton.setFillColor(sf::Color::Yellow);
            else
                placeBetButton.setFillColor(sf::Color::White);
        }
        else {
            // otherwise highlight Deal when not in round
            if (!roundInProgress) {
                if (dealButton.getGlobalBounds().contains(mf))
                    dealButton.setFillColor(sf::Color::Yellow);
                else
                    dealButton.setFillColor(sf::Color::White);
            }
        }
        // When round is in progress and it's the human's turn, highlight human action buttons.
        if (roundInProgress && currentPlayerTurn == humanIndex) {
            if (hitButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                hitButton.setFillColor(sf::Color::Yellow);
            }
            else {
                hitButton.setFillColor(sf::Color::White);
            }
            if (standButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                standButton.setFillColor(sf::Color::Yellow);
            }
            else {
                standButton.setFillColor(sf::Color::White);
            }
            if (doubleButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                doubleButton.setFillColor(sf::Color::Yellow);
            }
            else {
                doubleButton.setFillColor(sf::Color::White);
            }
            if (splitButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                splitButton.setFillColor(sf::Color::Yellow);
            }
            else {
                splitButton.setFillColor(sf::Color::White);
            }
        }
    }

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left &&
        readyToBet && bettingPhase)
    {
        sf::Vector2i mp = sf::Mouse::getPosition(window);
        sf::Vector2f mf(mp);
        if (sliderKnob.getGlobalBounds().contains(mf) ||
            sliderTrack.getGlobalBounds().contains(mf))
        {
            isDraggingSlider = true;
        }
    }


    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {

        isDraggingSlider = false;

        if (isTextClicked(quitButton, window)) {
            selectedIndex = 1;
            return;
        }

        if (!roundInProgress && readyToBet && bettingPhase) {
            if (isTextClicked(placeBetButton, window)) {
                bettingPhase = false;
                readyToBet = false;
                startNewRound();
                countText.setPosition(10.f, 10.f);
            }
            return;
        }

        if (!roundInProgress && !readyToBet && !bettingPhase) {
            if (isTextClicked(dealButton, window)) {
                readyToBet = true;
                bettingPhase = true;
                countText.setFont(font);
                countText.setCharacterSize(30);
                countText.setFillColor(sf::Color::White);
                countText.setPosition(
                    remainingMoneyText.getPosition().x,
                    remainingMoneyText.getPosition().y + remainingMoneyText.getCharacterSize() + 10.f
                );
                countText.setString("Count: " + std::to_string(counter.getCount()));
                for (auto& v : playersCardSprites)
                    v.clear();
                dealerCardSprites.clear();
                message = "";
                messageText.setString("");

                currentBetAmount = minBet;
                float left = sliderTrack.getPosition().x;
                float midY = sliderTrack.getPosition().y + sliderTrack.getSize().y / 2.f;
                sliderKnob.setPosition(left, midY);
                sliderLabel.setString("Bet: " + std::to_string(currentBetAmount));

                int rem = players[humanIndex].getBalance() - currentBetAmount;
                remainingMoneyText.setString("Remaining: " + std::to_string(rem));
            }
            return;
        }

        if (roundInProgress && currentPlayerTurn == humanIndex) {
            if (isTextClicked(hitButton, window)) {
                players[humanIndex].hit(deck, counter);
                if (players[humanIndex].isBusted()) {
                    message = "You busted!";
                    finishRound();
                }
                updateDisplay();
            }
            else if (isTextClicked(standButton, window)) {
                finishRound();
                updateDisplay();
            }
            else if (isTextClicked(doubleButton, window)) {
                if (players[humanIndex].getCurrentBet() <= players[humanIndex].getBalance()) {
                    players[humanIndex].placeBet(players[humanIndex].getCurrentBet());
                    players[humanIndex].hit(deck, counter);
                    finishRound();
                }
                updateDisplay();
            }
            else if (isTextClicked(splitButton, window)) {
                if (players[humanIndex].split()) {
                    if (players[humanIndex].getCurrentHand().getCards().size() < 2)
                        players[humanIndex].hit(deck, counter);
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
        // Keyboard controls for the human player.
        if (!roundInProgress) {
            if (event.key.code == sf::Keyboard::D)
                startNewRound();
        }
        else {
            if (currentPlayerTurn == humanIndex) {
                switch (event.key.code) {
                case sf::Keyboard::H:
                    players[humanIndex].hit(deck, counter);
                    if (players[humanIndex].isBusted()) {
                        message = "You busted!";
                        finishRound();
                    }
                    updateDisplay();
                    break;
                case sf::Keyboard::S:
                    finishRound();
                    updateDisplay();
                    break;
                case sf::Keyboard::P:
                    if (players[humanIndex].split()) {
                        if (players[humanIndex].getCurrentHand().getCards().size() < 2)
                            players[humanIndex].hit(deck, counter);
                        updateDisplay();
                    }
                    else {
                        message = "Cannot split this hand.";
                        updateDisplay();
                    }
                    break;
                case sf::Keyboard::X:
                    if (players[humanIndex].getCurrentBet() <= players[humanIndex].getBalance()) {
                        players[humanIndex].placeBet(players[humanIndex].getCurrentBet());
                        players[humanIndex].hit(deck, counter);
                        finishRound();
                    }
                    updateDisplay();
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void Game::draw(sf::RenderWindow& window) {
    // Draw all players' card sprites.
    for (const auto& vec : playersCardSprites) {
        for (const auto& sprite : vec)
            window.draw(sprite);
    }
    // Draw dealer's card sprites.
    for (const auto& sprite : dealerCardSprites)
        window.draw(sprite);
    // Draw the message text.
    window.draw(messageText);
    // Draw human action buttons (or the Deal button) on top.
        // always draw quit
    window.draw(quitButton);

    if (!roundInProgress) {
        if (readyToBet && bettingPhase) {
            window.draw(placeBetButton);
            window.draw(sliderTrack);
            window.draw(sliderKnob);
            window.draw(sliderLabel);
            window.draw(remainingMoneyText);
            window.draw(countText);
        }
        else {
            window.draw(dealButton);
        }
    }

    else if (roundInProgress && currentPlayerTurn != humanIndex) {
        int turnsLeft = std::max(0, humanIndex - static_cast<int>(currentPlayerTurn));
        turnsUntilPlayText.setString("Turns till play: " + std::to_string(turnsLeft));
        window.draw(turnsUntilPlayText);
    }

    else if (roundInProgress && currentPlayerTurn == humanIndex) {
        window.draw(hitButton);
        window.draw(standButton);
        window.draw(doubleButton);
        window.draw(splitButton);
    }

    if (!countText.getString().isEmpty())
        window.draw(countText);

    // Update bot moves after drawing.
    updateBotMoves();
}

Game::Option Game::getSelectedOption() const {
    switch (selectedIndex) {
    case 0: return Option::DEAL;
    case 1: return Option::QUIT;
    default: return Option::NONE;
    }
}

sf::FloatRect Game::getSelectedOptionPos() const {
    return quitButton.getGlobalBounds();
}