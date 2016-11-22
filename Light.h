#pragma once
#include <ltbl/lighting/LightSystem.h>
#include <SFML/Graphics.hpp>
#include "Var.h"
struct Lights
{
	Lights(sf::Vector2f const& tileSize, Objects const& objects);
	ltbl::LightSystem ls;
	sf::Shader unshadowShader;
	sf::Shader lightOverShapeShader;
	sf::Sprite Lsprite;
	sf::RenderStates lightRenderStates;
	std::shared_ptr<ltbl::LightPointEmission> flashLight;
	sf::Texture pointLightTexture;
	sf::Texture ConeLightTexture;
	sf::Texture  penumbraTexture;

    void AddShadowShapes(Objects const& objects);
};
