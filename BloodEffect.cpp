#include "stdafx.h"
#include "BloodEffect.h"

void BloodEffect::InitializeBloodEffect(sf::Texture const& texture)
{
    bloodSprite.setTexture(texture);
    rect = sf::FloatRect(0.f, 47.f, 39.f, 47.f);
    bloodSprite.setOrigin(rect.width / 2.f, rect.height / 2.f);
    currentFrame = 0;
}

void UpdateBloodEffect(BloodEffect & effect, Enemy & enemy)
{
    effect.currentFrame += 0.016f * 100.f / 3.f;
    if (effect.currentFrame > effect.countFrame)
    {
        effect.currentFrame -= effect.countFrame;
        enemy.isBloodEffect = false;
    }
    sf::Vector2f enemyPosition = enemy.enemySprite.getPosition();
    effect.bloodSprite.setTextureRect(sf::IntRect(0, effect.rect.height * int(effect.currentFrame), effect.rect.width, effect.rect.height));
}
