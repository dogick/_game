#pragma once
#include "Enemy.h"

struct BloodEffect
{
    sf::Sprite bloodSprite;
    float currentFrame ;
    const int countFrame = 15;;
    sf::FloatRect rect;

    void InitializeBloodEffect(sf::Texture const& texture);
};

void UpdateBloodEffect(BloodEffect & effect, Enemy & enemy);