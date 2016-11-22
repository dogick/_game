#include "stdafx.h"
#include "Bullet.h"
#include <iostream>

Bullet::Bullet(sf::Texture const& bulletTexture, sf::Vector2f const& bulletPosition, sf::Vector2f &Direction, float Rotation, float Damage)
{
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setPosition(bulletPosition);
    bulletSprite.setRotation(Rotation);
    direction = Direction;
    rotation = Rotation;
    damage = Damage;
}

sf::FloatRect GetBulletRect(sf::Sprite const& bullet)
{
    return sf::FloatRect(bullet.getPosition().x, bullet.getPosition().y, 5, 5);
}

bool CheckCollisionWithMap(sf::Sprite const& bullet, Objects const& barriers)
{
    for (auto barrier : barriers)
    {
        if (GetBulletRect(bullet).intersects(static_cast<sf::FloatRect>(barrier.rect)) && (barrier.name == "solid"))
        {
            return true;
        }
    }
    return false;
}

bool CheckCollisionWithEnemy(sf::Sprite const& bullet, std::vector<Enemy> & enemys)
{
    for (auto & enemy : enemys)
    {
        if (GetBulletRect(bullet).intersects(enemy.GetEnemyRect()) && (enemy.action != ActionEnemy::NONE))
        {
            enemy.health -= 10.f;
            enemy.isBloodEffect = true;
            if ((enemy.health < 0) && (enemy.action != ActionEnemy::DIE))
            {
                enemy.currentFrame = 0;
                enemy.action = ActionEnemy::DIE;
            }
            return true;
        }
    }
    return false;
}

void UpdateBullet(std::vector<Bullet*> & bullets, Objects const& objects,  std::vector<Enemy> & enemys)
{
    for (auto it = bullets.begin(); it != bullets.end();)
    {
        auto *bullet = *it;
        bool isCollisionWithEnemy = CheckCollisionWithEnemy(bullet->bulletSprite, enemys);
        if (!CheckCollisionWithMap(bullet->bulletSprite, objects) && !isCollisionWithEnemy)
        {
            float offsetX = bullet->speed  * bullet->direction.x;
            float offsetY = bullet->speed  * bullet->direction.y;
            bullet->bulletSprite.move(offsetX, offsetY);
            ++it;
        }
        else
        {
            it = bullets.erase(it);
        }
    }

}

