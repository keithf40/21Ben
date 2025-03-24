#pragma once
#include <SFML/Graphics.hpp>

class Texture
{
public:
    // Hearts
    sf::Texture hearts2Texture;
    sf::Texture hearts3Texture;
    sf::Texture hearts4Texture;
    sf::Texture hearts5Texture;
    sf::Texture hearts6Texture;
    sf::Texture hearts7Texture;
    sf::Texture hearts8Texture;
    sf::Texture hearts9Texture;
    sf::Texture hearts10Texture;
    sf::Texture heartsjackTexture;
    sf::Texture heartsqueenTexture;
    sf::Texture heartskingTexture;
    sf::Texture heartsaceTexture;

    // Diamonds
    sf::Texture diamonds2Texture;
    sf::Texture diamonds3Texture;
    sf::Texture diamonds4Texture;
    sf::Texture diamonds5Texture;
    sf::Texture diamonds6Texture;
    sf::Texture diamonds7Texture;
    sf::Texture diamonds8Texture;
    sf::Texture diamonds9Texture;
    sf::Texture diamonds10Texture;
    sf::Texture diamondsjackTexture;
    sf::Texture diamondsqueenTexture;
    sf::Texture diamondskingTexture;
    sf::Texture diamondsaceTexture;

    // Spades
    sf::Texture spades2Texture;
    sf::Texture spades3Texture;
    sf::Texture spades4Texture;
    sf::Texture spades5Texture;
    sf::Texture spades6Texture;
    sf::Texture spades7Texture;
    sf::Texture spades8Texture;
    sf::Texture spades9Texture;
    sf::Texture spades10Texture;
    sf::Texture spadesjackTexture;
    sf::Texture spadesqueenTexture;
    sf::Texture spadeskingTexture;
    sf::Texture spadesaceTexture;

    // Clubs
    sf::Texture clubs2Texture;
    sf::Texture clubs3Texture;
    sf::Texture clubs4Texture;
    sf::Texture clubs5Texture;
    sf::Texture clubs6Texture;
    sf::Texture clubs7Texture;
    sf::Texture clubs8Texture;
    sf::Texture clubs9Texture;
    sf::Texture clubs10Texture;
    sf::Texture clubsjackTexture;
    sf::Texture clubsqueenTexture;
    sf::Texture clubskingTexture;
    sf::Texture clubsaceTexture;

    // Back of card
    sf::Texture backOfCard;

    // Backgrounds
    sf::Texture menuBackgroundImage;
    sf::Texture settingsBackgroundImage;
    sf::Texture gameBackgroundImage;

    void loadTextures();

};

