#include "stdafx.h"
#include "Game.h"

void InitializeGame(Game & game)
{
    game.level.LoadFromFile("map/map.tmx");
    game.barrier = game.level.GetObjects("solid");
    //Objects enemy = game.level.GetObjects("enemy");
    //game.barrier.insert(game.barrier.end(), enemy.begin(), enemy.end());
    game.light = new Lights(sf::Vector2f(static_cast<float>(game.level.GetTileSize().x), static_cast<float>(game.level.GetTileSize().y)), game.barrier);
    game.view.reset(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    game.window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game");
    game.viewLight = game.window.getDefaultView();
    game.textures.LoadTextureFiles();
    game.bloodEffect.InitializeBloodEffect(game.textures.bloodTexture);
    game.player.playerSound.LoadSoundFiles();
    InitializePlayer(game.player, game.textures);
    InitializeEnemys(game.enemys, game.level.GetObjects("enemy"), game.textures);
    game.bulletSprite.setTexture(game.textures.bulletTexture);
    ReplaceCursor(game.textures.cursorTexture, game.cursorSprite);
}

void ResizeWindowGame(sf::RenderWindow & window)
{
    float windowWidth = static_cast<float>(window.getSize().x);
    float windowHeight = static_cast<float>(window.getSize().y);
    window.setView(sf::View(sf::FloatRect(0, 0, windowWidth, windowHeight)));
}

void UpdateCursorPosition(sf::RenderWindow & window, sf::Sprite &cursorSprite)
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f pos = window.mapPixelToCoords(pixelPos);
    cursorSprite.setPosition(static_cast<sf::Vector2f>(pos));
}

void ReplaceCursor(sf::Texture &cursorTexture, sf::Sprite &cursorSprite)
{
    cursorSprite.setTexture(cursorTexture);
    cursorSprite.setOrigin(cursorTexture.getSize().x / 2.f, cursorTexture.getSize().y / 2.f);
    cursorSprite.setPosition(100, 100);
}

void HandleEvents(sf::RenderWindow & window, Player &player)
{

    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Resized:
            std::cout << "RESIZE" << std::endl;
            ResizeWindowGame(window);
            break;
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            HandlePlayerKeyPress(event.key, player);
            break;
        case sf::Event::KeyReleased:
            HandlePlayerKeyRelease(event.key, player);
        default:
            break;
        }
        if ((event.key.code == sf::Mouse::Left) && (event.type == sf::Event::MouseButtonPressed))
        {
            player.isShot = true;

        }
        else  if ((event.key.code == sf::Mouse::Left) && (event.type == sf::Event::MouseButtonReleased))
        {
            player.isShot = false;
        }
    }
}

void Update(Game &game, float elapsedTime)
{
    game.player.mousePosition = GetMousePosition(game.window);
    UpdatePlayer(game.player, elapsedTime, game.barrier, game.textures.bulletTexture);
    UpdateEnemys(game.enemys, elapsedTime, game.barrier, game.player.playerSprite.getPosition());
    UpdateCursorPosition(game.window, game.cursorSprite);
    UpdateBullet(game.player.bullets, game.barrier, game.enemys);
}



void RenderBullets(Game &game)
{
    for (auto bullet : game.player.bullets)
    {
        game.window.draw(bullet->bulletSprite);
    }
}

void RenderEnemys(Game &game)
{
    for (auto & enemy : game.enemys)
    {
        if (int(enemy.action) != 0)
        {
            game.window.draw(enemy.enemySprite);
        }
        if (enemy.isBloodEffect)
        {
            UpdateBloodEffect(game.bloodEffect, enemy);
            game.bloodEffect.bloodSprite.setPosition(enemy.enemySprite.getPosition());
            game.window.draw(game.bloodEffect.bloodSprite);
        }
    }
}

void RenderLight(Game & game, sf::RenderWindow & window)
{
    window.setView(game.viewLight);
    game.light->flashLight->_emissionSprite.setPosition(game.player.playerSprite.getPosition());
    game.light->flashLight->_emissionSprite.setRotation(game.player.playerSprite.getRotation());
    game.light->ls.render(game.view, game.light->unshadowShader, game.light->lightOverShapeShader);
    game.light->Lsprite.setTexture(game.light->ls.getLightingTexture());
    game.light->lightRenderStates.blendMode = sf::BlendMultiply; //фон
    window.draw(game.light->Lsprite, game.light->lightRenderStates);
}

void Render(sf::RenderWindow & window,  Game &game)
{
    window.clear();
    game.level.Draw(window);
    RenderBullets(game);
    RenderEnemys(game);
    window.draw(game.player.playerSprite);
    RenderLight(game, window);
    window.setView(game.view);
    window.draw(game.cursorSprite);
    /*sf::RectangleShape shape1;
    shape1.setSize(sf::Vector2f(40, 40));
    sf::IntRect textureRect = playerSprite.getTextureRect();
    shape1.setPosition(playerSprite.getPosition().x  -PLAYER_SIZE.x / 2, playerSprite.getPosition().y - PLAYER_SIZE.x / 2);
    shape1.setFillColor(sf::Color(255,255,255, 60));
    window.draw(shape1);*/
    window.display();
}

void GetPlayerCoordinateForView(sf::View & view, sf::Vector2f playerPosition)
{
    int tempX = int(playerPosition.x);
    int tempY = int(playerPosition.y);

    if (tempX < 1000) tempX = 1000;
    if (tempY < 400) tempY = 400;

    view.setCenter(float(tempX), float(tempY));

}
