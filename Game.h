#pragma once
#include <ltbl/lighting/LightSystem.h>
#include "Player.h"
#include "Enemy.h"
#include "level.h"
#include "Config.h"
#include "Light.h"
#include "Var.h"
#include "BloodEffect.h"

struct Game
{
    Level level;
    sf::View view;
    sf::View viewLight;
    Lights *light;
    sf::RenderWindow window;
    Player player;
    TextureGame textures;
    Objects barrier;
    std::vector<Enemy> enemys;
    sf::Sprite cursorSprite;
    sf::Sprite bulletSprite;
    BloodEffect bloodEffect;
};

void InitializeGame(Game & game);
void ResizeWindowGame(sf::RenderWindow & window);
void ReplaceCursor(sf::Texture &cursorTexture, sf::Sprite &cursorSprite);
void UpdateCursorPosition(sf::RenderWindow & window, sf::Sprite &cursorSprite);
void HandleEvents(sf::RenderWindow & window, Player &player);
void Update(Game &game, float elapsedTime);
void Render(sf::RenderWindow & window, Game &game);
void GetPlayerCoordinateForView(sf::View & view, sf::Vector2f playerPosition);