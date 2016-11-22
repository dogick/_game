#pragma once
#include <SFML/Audio.hpp>
#include "Texture.h"
#include "Bullet.h"
#include <iostream> 
#include <vector>
#include "level.h"
#include "Var.h"

struct PlayerSound
{
	sf::SoundBuffer soundShotgunShotBuffer;
	sf::Sound soundShotgunShot;

	sf::SoundBuffer soundM4A1ShotBuffer;
	sf::Sound soundM4A1Shot;

	void LoadSoundFiles();
};

enum struct Arms
{
	SHOTGUN,
	M4A1
};

enum CountFrameThePlayer
{
	CF_STAND = 20,
	CF_RUN = 16,
	CF_RUN_SHOT_SHOTGUN = 20,
	CF_STAND_SHOT_M4A1 = 5,
	CF_RUN_SHOT_M4A1 = 12,
};

enum struct Direction
{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

struct Player
{
	sf::Sprite playerSprite;
	Direction direction;
	sf::Vector2f mousePosition;

	float currentFrame;

	bool isShot;
	Arms weapon;
	PlayerSound playerSound;

	std::vector<Bullet*> bullets;
	float bulletTime;

    void CheckCollisionWithMap(sf::Vector2f & speed, Objects const& barrier);
	sf::FloatRect GetPlayerRect();
};

void InitializePlayer(Player & player, TextureGame & texture);
void HandlePlayerKeyPress(const sf::Event::KeyEvent &event, Player &player);
void HandlePlayerKeyRelease(const sf::Event::KeyEvent &event, Player &player);
void UpdatePlayer(Player &player, float elapsedTime, Objects const& object, sf::Texture const& bulletTexture);
sf::Vector2f GetMousePosition(sf::RenderWindow &window);
void InitializePlayerSound(Player &player);