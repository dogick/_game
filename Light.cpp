#include "stdafx.h"
#include "Light.h"
#include "Var.h"
#include "Config.h"
#include <iostream>

Lights::Lights(sf::Vector2f const& tileSize, Objects const& objects)
{
	penumbraTexture.loadFromFile("light/penumbraTexture.png");
	penumbraTexture.setSmooth(true);
	ConeLightTexture.loadFromFile("light/spotLightTexture.png");
	ConeLightTexture.setSmooth(true);
	unshadowShader.loadFromFile("light/unshadowShader.vert", "light/unshadowShader.frag");
	lightOverShapeShader.loadFromFile("light/lightOverShapeShader.vert", "light/lightOverShapeShader.frag");
	ls.create(ltbl::rectFromBounds(-tileSize, tileSize), sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT), penumbraTexture, unshadowShader, lightOverShapeShader);
	flashLight = std::make_shared<ltbl::LightPointEmission>();
	flashLight->_emissionSprite.setOrigin(sf::Vector2f(ConeLightTexture.getSize().x * 0.5f, ConeLightTexture.getSize().y * 0.5f));
	flashLight->_emissionSprite.setTexture(ConeLightTexture);//Текстура свечения как у фонаря.
	flashLight->_emissionSprite.setScale(sf::Vector2f(20, 10));
	flashLight->_emissionSprite.setColor(sf::Color(255, 255, 255));
	ls.addLight(flashLight);
    AddShadowShapes(objects);
}

void Lights::AddShadowShapes(Objects const& objects)
{
    for (auto object : objects)
    {
        std::shared_ptr<ltbl::LightShape> lightShape = std::make_shared<ltbl::LightShape>();
        lightShape->_shape.setPointCount(4);
        sf::IntRect shape = object.rect;
        lightShape->_shape.setPoint(0, sf::Vector2f(0, 0));
        lightShape->_shape.setPoint(1, sf::Vector2f(shape.width, 0));
        lightShape->_shape.setPoint(2, sf::Vector2f(shape.width, shape.height));
        lightShape->_shape.setPoint(3, sf::Vector2f(0, shape.height));
        if (object.type == "light")
        {
            lightShape->_renderLightOverShape = false;
        }
        lightShape->_shape.setPosition(shape.left, shape.top);
        ls.addShape(lightShape);
    }
}
