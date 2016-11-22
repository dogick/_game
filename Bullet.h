#pragma once
#include "Var.h"
#include "Enemy.h"
#include "BloodEffect.h"

struct Bullet
{
	Bullet(sf::Texture const& bulletTexture, sf::Vector2f const& bulletPosition, sf::Vector2f &Direction, float Rotation, float Damage);
    sf::Sprite bulletSprite;
	sf::Vector2f position;
	sf::Vector2f direction;
	float rotation;
	float damage;
	float speed = 30;

};

sf::FloatRect GetBulletRect(sf::Sprite const& bullet);
bool CheckCollisionWithMap(sf::Sprite const& bullet, Objects const& barriers);
bool CheckCollisionWithEnemy(sf::Sprite const& bullet, std::vector<Enemy> & enemys);
void UpdateBullet(std::vector<Bullet*> & bullets, Objects const& objects, std::vector<Enemy> & enemys);