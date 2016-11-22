#pragma once
#include "Var.h"
#include "Config.h"
#include "Texture.h"

enum class ActionEnemy 
{
    NONE,
    GET_OUT = 30,
    STEP = 36,
    MOVE = 20,
    ATTACKS,
    DIE = 18,
};

static const std::map<ActionEnemy, sf::IntRect> TextureRect = {
    { ActionEnemy::STEP, sf::IntRect(0, 57, 50, 57) },
    { ActionEnemy::MOVE, sf::IntRect(50, 62, 81, 62) },
    { ActionEnemy::GET_OUT, sf::IntRect(157, 64, 82, 64) },
    { ActionEnemy::DIE, sf::IntRect(265, 84, 107, 84) },
    //{ ActionEnemy::ATTACKS, sf::IntRect(0, 57, 50, 57) },
};

struct Enemy
{
    sf::Sprite enemySprite;

    sf::Vector2f direction;
    float currentFrame;

    bool isLive;
    ActionEnemy action;
    //void CheckCollisionWithMap(sf::Vector2f & speed, Objects const& barrier);
    sf::FloatRect GetEnemyRect();
    float health;
    bool isBloodEffect;
};

void InitializeEnemys(std::vector<Enemy> & enemys, Objects const& positionEnemies, TextureGame const& texture);
void UpdateEnemyFrame(Enemy & enemy, float elapsedTime);
void UpdateEnemys(std::vector<Enemy> & enemys, float elapsedTime, Objects const& barriers, sf::Vector2f const& playerPos);