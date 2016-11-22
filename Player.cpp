#include "stdafx.h"
#include "Player.h"
#include "Config.h"
#include "Var.h"

void InitializePlayer(Player & player, TextureGame & texture)
{
	player.playerSprite.setTexture(texture.playerTexture);
	InitializePlayerSound(player);
	player.playerSprite.setTextureRect(sf::IntRect(150, 121, 150, 121));
	player.playerSprite.setPosition(450, 250);
    sf::IntRect textureRect = player.playerSprite.getTextureRect();
    player.playerSprite.setOrigin(textureRect.width / 2.f, textureRect.height / 2.f);
	player.direction = Direction::NONE;
	player.currentFrame = 0;
	player.isShot = false;
	player.weapon = Arms::SHOTGUN;
	player.bulletTime = 0;
}

void PlayerSound::LoadSoundFiles()
{
	soundShotgunShotBuffer.loadFromFile("resources/sound/shotgun.ogg");
	soundM4A1ShotBuffer.loadFromFile("resources/sound/shoot.ogg");
}

void InitializePlayerSound(Player & player)
{
	player.playerSound.soundShotgunShot.setBuffer(player.playerSound.soundShotgunShotBuffer);
	player.playerSound.soundM4A1Shot.setBuffer(player.playerSound.soundM4A1ShotBuffer);
}


sf::Vector2f GetMousePosition(sf::RenderWindow &window)
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	return window.mapPixelToCoords(pixelPos);
}

void RotatePlayer(Player &player) 
{
	float dx = player.mousePosition.x - player.playerSprite.getPosition().x;
	float dy = player.mousePosition.y - player.playerSprite.getPosition().y;
	float rotation = (atan2(dy, dx)) * 180 / PI;
	player.playerSprite.setRotation(rotation);
}

void ShootFromM4A1(Player &player, sf::Vector2f &vectorNormal, sf::Texture const& bulletTexture)
{
	if (player.isShot && (player.weapon == Arms::M4A1) && (player.bulletTime > 0.2))
	{
		player.playerSound.soundM4A1Shot.play();
		player.bullets.push_back(new Bullet(bulletTexture, player.playerSprite.getPosition(), vectorNormal + sf::Vector2f(0, 0), player.playerSprite.getRotation(), static_cast<float>(Arms::M4A1)));
		player.bulletTime = 0;
	}
}

void ShootFromShotgun(Player &player, sf::Vector2f const& bulletDirection, sf::Texture const& bulletTexture)
{
	if (player.isShot && (player.weapon == Arms::SHOTGUN))
	{
		player.playerSound.soundShotgunShot.play();
        player.bullets.push_back(new Bullet(bulletTexture, player.playerSprite.getPosition(), bulletDirection + sf::Vector2f(0.1f, 0.1f), player.playerSprite.getRotation(), static_cast<float>(Arms::SHOTGUN)));
		player.bullets.push_back(new Bullet(bulletTexture, player.playerSprite.getPosition(), bulletDirection + sf::Vector2f(0, 0), player.playerSprite.getRotation(), static_cast<float>(Arms::SHOTGUN)));
		player.bullets.push_back(new Bullet(bulletTexture, player.playerSprite.getPosition(), bulletDirection + sf::Vector2f(-0.1f, -0.1f), player.playerSprite.getRotation(), static_cast<float>(Arms::SHOTGUN)));
		player.bulletTime = 0;

	}
}

void UpdatePlayerFrame(Player &player, float elapsedTime, sf::Vector2f &bulletDirection, sf::Texture const& bulletTexture)
{
    sf::Vector2i textureRect(0, 122);
    int amountFrame = CF_RUN;
    if ((player.direction == Direction::NONE) && (player.isShot))
    {
        if (player.weapon == Arms::SHOTGUN)
        {
            textureRect = sf::Vector2i(300, 122);
            amountFrame = CF_RUN_SHOT_SHOTGUN;
        }
        else
        {
            textureRect = sf::Vector2i(750, 122);
            amountFrame = CF_STAND_SHOT_M4A1;
        }
    }
    else if(player.isShot)
    {
        if (player.weapon == Arms::SHOTGUN)
        {
            textureRect = sf::Vector2i(450, 122);
            amountFrame = CF_RUN_SHOT_SHOTGUN;
        }
        else
        {
            textureRect = sf::Vector2i(600, 122);
            amountFrame = CF_RUN_SHOT_M4A1;
        }
    }
    else if(player.direction == Direction::NONE)
    {
        textureRect = sf::Vector2i(150, 122);
        amountFrame = CF_STAND;
    }

    player.currentFrame += elapsedTime * PLAYER_SPEED / 4.f;
    ShootFromM4A1(player, bulletDirection, bulletTexture);
    if (player.currentFrame > amountFrame)
    {
        ShootFromShotgun(player, bulletDirection, bulletTexture);
        player.currentFrame -= amountFrame;
    }
    player.playerSprite.setTextureRect(sf::IntRect(textureRect.x, textureRect.y * int(player.currentFrame), 150, 122));
}

sf::FloatRect Player::GetPlayerRect()
{  
	return sf::FloatRect(playerSprite.getPosition().x - PLAYER_SIZE.x / 2, playerSprite.getPosition().y - PLAYER_SIZE.y / 2, PLAYER_SIZE.x, PLAYER_SIZE.y);
}

void Player::CheckCollisionWithMap(sf::Vector2f & speed, Objects const& barriers)
{
	for (auto barrier : barriers)
	{
        if (GetPlayerRect().intersects(static_cast<sf::FloatRect>(barrier.rect)) && (barrier.name == "solid"))
        {
            sf::Vector2f playerPosition = playerSprite.getPosition();
            if ((playerPosition.x < barrier.rect.left) && ((mousePosition.x > barrier.rect.left) || (mousePosition.x < barrier.rect.left) && (speed.x > 0))) //слева
            {
                speed = sf::Vector2f(0, speed.y);
            }
            else if ((playerPosition.x > barrier.rect.left + barrier.rect.width) && ((mousePosition.x < barrier.rect.left) || (mousePosition.x > barrier.rect.left) && (speed.x < 0)))
            {
                speed = sf::Vector2f(0, speed.y);
            }
            else if ((playerPosition.y > barrier.rect.top) && ((mousePosition.y < barrier.rect.top) || (mousePosition.y > barrier.rect.top) && (speed.y < 0))) //снизу
            {
                speed = sf::Vector2f(speed.x, 0);
            }
            else if ((playerPosition.y < barrier.rect.top) && ((mousePosition.y > barrier.rect.top) || (mousePosition.y < barrier.rect.top) && (speed.y > 0))) //сверху
            {
                speed = sf::Vector2f(speed.x, 0);
            }
        }
	}
}
void UpdatePlayer(Player &player, float elapsedTime, Objects const& barriers, sf::Texture const& bulletTexture)
{
	const float speed = PLAYER_SPEED * elapsedTime;
	player.bulletTime += elapsedTime;
	RotatePlayer(player);

	float dx = player.mousePosition.x - player.playerSprite.getPosition().x;
	float dy = player.mousePosition.y - player.playerSprite.getPosition().y;
	float distance = hypot(dx, dy);
	sf::Vector2f vectorNormal = sf::Vector2f(dx, dy)/ distance;

	sf::Vector2f movement(0, 0);
	if (distance < 25)
	{
		player.direction = Direction::NONE;
	}
	switch (player.direction)
	{
	case Direction::UP:
        movement = sf::Vector2f(speed *  vectorNormal.x, speed *  vectorNormal.y);
	break;
	case Direction::DOWN:
        movement = sf::Vector2f(-speed * vectorNormal.x, -speed * vectorNormal.y);
	break;
	case Direction::LEFT:
        movement = sf::Vector2f(speed * std::sin(PI * player.playerSprite.getRotation() / 180.f), -std::cos(PI * player.playerSprite.getRotation() / 180.f) *speed );
	break;
	case Direction::RIGHT:
        movement = sf::Vector2f(-speed * std::sin(PI * player.playerSprite.getRotation() / 180.f), std::cos(PI * player.playerSprite.getRotation() / 180.f) *speed);
	break;
	}
	UpdatePlayerFrame(player, elapsedTime, vectorNormal, bulletTexture);
	player.CheckCollisionWithMap(movement, barriers);
	player.playerSprite.move(movement);
}

void HandlePlayerKeyPress(const sf::Event::KeyEvent &event, Player &player)
{

	switch (event.code)
	{
	case sf::Keyboard::W:
		player.direction = Direction::UP;
		break;
	case sf::Keyboard::S:
		player.direction = Direction::DOWN;
		break;
	case sf::Keyboard::A:
		player.direction = Direction::LEFT;
		break;
	case sf::Keyboard::D:
		player.direction = Direction::RIGHT;
		break;
	case sf::Keyboard::Q:
		player.weapon = Arms::M4A1;
		break;
	case sf::Keyboard::E:
		player.weapon = Arms::SHOTGUN;
		break;
	default:
		player.direction = Direction::NONE;
		break;
	}
}

void HandlePlayerKeyRelease(const sf::Event::KeyEvent &event, Player &player)
{
	bool handled = true;
	switch (event.code)
	{
	case sf::Keyboard::W:
		if (player.direction == Direction::UP)
		{
			player.direction = Direction::NONE;
		}
		break;
	case sf::Keyboard::S:
		if (player.direction == Direction::DOWN)
		{
			player.direction = Direction::NONE;
		}
		break;
	case sf::Keyboard::A:
		if (player.direction == Direction::LEFT)
		{
			player.direction = Direction::NONE;
		}
		break;
	case sf::Keyboard::D:
		if (player.direction == Direction::RIGHT)
		{
			player.direction = Direction::NONE;
		}
		break;
	default:
		handled = false;
		break;
	}
}
