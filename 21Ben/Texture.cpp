#include "Texture.h"

// Texture Loading Function
void Texture::loadTextures() {
    // Hearts
    hearts2Texture.loadFromFile("assets/2_of_hearts.png");
    hearts3Texture.loadFromFile("assets/3_of_hearts.png");
    hearts4Texture.loadFromFile("assets/4_of_hearts.png");
    hearts5Texture.loadFromFile("assets/5_of_hearts.png");
    hearts6Texture.loadFromFile("assets/6_of_hearts.png");
    hearts7Texture.loadFromFile("assets/7_of_hearts.png");
    hearts8Texture.loadFromFile("assets/8_of_hearts.png");
    hearts9Texture.loadFromFile("assets/9_of_hearts.png");
    hearts10Texture.loadFromFile("assets/10_of_hearts.png");
    heartsjackTexture.loadFromFile("assets/jack_of_hearts.png");
    heartsqueenTexture.loadFromFile("assets/queen_of_hearts.png");
    heartskingTexture.loadFromFile("assets/king_of_hearts.png");
    heartsaceTexture.loadFromFile("assets/ace_of_hearts.png");

    // Diamonds
    diamonds2Texture.loadFromFile("assets/2_of_diamonds.png");
    diamonds3Texture.loadFromFile("assets/3_of_diamonds.png");
    diamonds4Texture.loadFromFile("assets/4_of_diamonds.png");
    diamonds5Texture.loadFromFile("assets/5_of_diamonds.png");
    diamonds6Texture.loadFromFile("assets/6_of_diamonds.png");
    diamonds7Texture.loadFromFile("assets/7_of_diamonds.png");
    diamonds8Texture.loadFromFile("assets/8_of_diamonds.png");
    diamonds9Texture.loadFromFile("assets/9_of_diamonds.png");
    diamonds10Texture.loadFromFile("assets/10_of_diamonds.png");
    diamondsjackTexture.loadFromFile("assets/jack_of_diamonds.png");
    diamondsqueenTexture.loadFromFile("assets/queen_of_diamonds.png");
    diamondskingTexture.loadFromFile("assets/king_of_diamonds.png");
    diamondsaceTexture.loadFromFile("assets/ace_of_diamonds.png");

    // Spades
    spades2Texture.loadFromFile("assets/2_of_spades.png");
    spades3Texture.loadFromFile("assets/3_of_spades.png");
    spades4Texture.loadFromFile("assets/4_of_spades.png");
    spades5Texture.loadFromFile("assets/5_of_spades.png");
    spades6Texture.loadFromFile("assets/6_of_spades.png");
    spades7Texture.loadFromFile("assets/7_of_spades.png");
    spades8Texture.loadFromFile("assets/8_of_spades.png");
    spades9Texture.loadFromFile("assets/9_of_spades.png");
    spades10Texture.loadFromFile("assets/10_of_spades.png");
    spadesjackTexture.loadFromFile("assets/jack_of_spades.png");
    spadesqueenTexture.loadFromFile("assets/queen_of_spades.png");
    spadeskingTexture.loadFromFile("assets/king_of_spades.png");
    spadesaceTexture.loadFromFile("assets/ace_of_spades.png");

    // Clubs
    clubs2Texture.loadFromFile("assets/2_of_clubs.png");
    clubs3Texture.loadFromFile("assets/3_of_clubs.png");
    clubs4Texture.loadFromFile("assets/4_of_clubs.png");
    clubs5Texture.loadFromFile("assets/5_of_clubs.png");
    clubs6Texture.loadFromFile("assets/6_of_clubs.png");
    clubs7Texture.loadFromFile("assets/7_of_clubs.png");
    clubs8Texture.loadFromFile("assets/8_of_clubs.png");
    clubs9Texture.loadFromFile("assets/9_of_clubs.png");
    clubs10Texture.loadFromFile("assets/10_of_clubs.png");
    clubsjackTexture.loadFromFile("assets/jack_of_clubs.png");
    clubsqueenTexture.loadFromFile("assets/queen_of_clubs.png");
    clubskingTexture.loadFromFile("assets/king_of_clubs.png");
    clubsaceTexture.loadFromFile("assets/ace_of_clubs.png");

    // Back of Card
    backOfCard.loadFromFile("assets/back_of_card.png");

    // Background
    menuBackgroundImage.loadFromFile("assets/mainMenuBackground.jpg");
    settingsBackgroundImage.loadFromFile("assets/settingsBackground.png");
    gameBackgroundImage.loadFromFile("assets/gameBackground.png");
}