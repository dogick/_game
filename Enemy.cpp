#include "stdafx.h"
#include "Enemy.h"
#include <iostream>

sf::FloatRect Enemy::GetEnemyRect()
{
    return sf::FloatRect(enemySprite.getPosition().x - ENEMY_SIZE.x / 2.f, enemySprite.getPosition().y - ENEMY_SIZE.y / 2.f, ENEMY_SIZE.x, ENEMY_SIZE.y);
}

void InitializeEnemys(std::vector<Enemy> & enemys, Objects const& positionEnemies, TextureGame const& texture)
{
    for (auto positionEnemy : positionEnemies)
    {
        Enemy enemy;
        enemy.currentFrame = 0;
        enemy.action = ActionEnemy::NONE;
        enemy.health = 100.f;
        enemy.isLive = true;
        enemy.isBloodEffect = false;
        enemy.enemySprite.setTexture(texture.enemyTexture);
        enemy.enemySprite.setPosition(positionEnemy.rect.left, positionEnemy.rect.top);
        enemys.push_back(enemy);
    }
}

void UpdateEnemyFrame(Enemy & enemy, float elapsedTime)
{
    sf::IntRect texture;
    auto it = TextureRect.find(enemy.action);
    if (it != TextureRect.end())
    {
        enemy.enemySprite.setOrigin(it->second.width / 2.f, it->second.height / 2.f);
        texture = it->second;
    }
    const int amountFrame = static_cast<int>(enemy.action);

    const float SPEED_ANIMATION = enemy.action == ActionEnemy::GET_OUT ? 6.f : 4.f; //скорость анимации 

    if ((enemy.action == ActionEnemy::DIE) && (int(enemy.currentFrame) == static_cast<int>(ActionEnemy::DIE)))
    {
       enemy.currentFrame = static_cast<int>(ActionEnemy::DIE);
    }
    else
    {
        enemy.currentFrame += elapsedTime * PLAYER_SPEED / SPEED_ANIMATION;
    }
    if (enemy.currentFrame > amountFrame)
    {
        if (enemy.action == ActionEnemy::GET_OUT) //враг передвигается после того как н
        {
            enemy.action = ActionEnemy::MOVE;
        }

        if (enemy.action != ActionEnemy::DIE)
        {
            enemy.currentFrame -= amountFrame;
        }
    }
    enemy.enemySprite.setTextureRect(sf::IntRect(texture.left, texture.top * int(enemy.currentFrame) , texture.width, texture.height));
}

void RotateEnemy(Enemy &enemy, sf::Vector2f const& playerPos)
{
    if (enemy.action != ActionEnemy::DIE)
    {
        float dx = playerPos.x - enemy.enemySprite.getPosition().x;
        float dy = playerPos.y - enemy.enemySprite.getPosition().y;
        float rotation = (atan2(dy, dx)) * 180 / PI;
        enemy.enemySprite.setRotation(rotation);
    }
}

void UpdateEnemys(std::vector<Enemy> & enemys, float elapsedTime, Objects const& barriers, sf::Vector2f const& playerPos)
{
    const float speed = ENEMY_SPEED * elapsedTime;
    sf::IntRect playerRect(playerPos.x - AREA_SIZE.x / 2.f, playerPos.y - AREA_SIZE.y / 2.f, AREA_SIZE.x, AREA_SIZE.y);
    for (auto & enemy : enemys)
    {
        sf::IntRect enemyRect(enemy.enemySprite.getPosition().x, enemy.enemySprite.getPosition().y, ENEMY_SIZE.x, ENEMY_SIZE.y);
        if (enemy.action != ActionEnemy::NONE)
        {
            RotateEnemy(enemy, playerPos);
            UpdateEnemyFrame(enemy, elapsedTime);
        }
        if (enemy.action == ActionEnemy::MOVE)
        {
            float dx = playerPos.x - enemy.enemySprite.getPosition().x;
            float dy = playerPos.y - enemy.enemySprite.getPosition().y;
            float normalization = std::hypot(dx, dy);
            sf::Vector2f movement(speed * dx / normalization, speed * dy / normalization);
            enemy.enemySprite.move(movement);
        } 
        else if ((enemy.action == ActionEnemy::NONE) && playerRect.intersects(enemyRect)) //из  земли
        {
            enemy.action = ActionEnemy::GET_OUT;
            enemy.enemySprite.setTextureRect(TextureRect.find(ActionEnemy::GET_OUT)->second); //для того чтобы не отрисовывалось весь список спрайтов
        }
        else if ((enemy.action == ActionEnemy::STEP) && playerRect.intersects(enemyRect))
        {
            enemy.action = ActionEnemy::MOVE;
            enemy.enemySprite.setTextureRect(TextureRect.find(ActionEnemy::GET_OUT)->second); //для того чтобы не отрисовывалось весь список спрайтов
        }
    }
}
