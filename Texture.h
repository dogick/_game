#pragma once
struct TextureGame
{
    sf::Texture playerTexture;
    sf::Texture cursorTexture;
    sf::Texture weaponTexture;
    sf::Texture bulletTexture;
    sf::Texture enemyTexture;
    sf::Texture bloodTexture;

    void LoadTextureFiles();
};