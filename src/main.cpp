#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "playerEntity.hpp"
#include "enemyEntity.hpp"
#include "animationComponent.hpp"
#include <systems/spawnComponent.hpp>

using namespace playerent;
using namespace enemyent;
using namespace movcomp;


bool circleIntersectsRectangle(const Transform& circleTransform, float circleRadius, const Transform& rectangleTransform) {
    float closestX = std::fmax(rectangleTransform.position.x, std::fmin(circleTransform.position.x, rectangleTransform.position.x + rectangleTransform.size.x));
    float closestY = std::fmax(rectangleTransform.position.y, std::fmin(circleTransform.position.y, rectangleTransform.position.y + rectangleTransform.size.y));

    float distanceX = circleTransform.position.x - closestX;
    float distanceY = circleTransform.position.y - closestY;
    float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

    return distance <= circleRadius;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    // W I N D O W  &  S E T T I N G S
    Transform windowEngine;
    windowEngine.position = { 0, 0 };
    windowEngine.size = { 800, 600 };

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow windowRender(
        sf::VideoMode(windowEngine.size.x, windowEngine.size.y),
        "Vampire Survival",
        sf::Style::Default,
        settings);
    windowRender.setVerticalSyncEnabled(true);

    // P L A Y E R
    animcomp::Animation animation;
    sf::Sprite playerSprite = animation.sprite;
    sf::Texture playerTexture;
    Player playerEngine;

    playerTexture.loadFromFile("assets/player.png");
    playerSprite.setTexture(playerTexture);

    playerEngine.PlayerTransform.position = { windowEngine.size.x / 2, windowEngine.size.y / 2 };
    playerSprite.setPosition(playerEngine.PlayerTransform.position.x, playerEngine.PlayerTransform.position.y);

    // B U L L E T S
    sf::CircleShape bulletRender(50.f);
    std::vector<Enemy> bulletsEngine;

    // T I M E R
    sf::Clock spawnClock;
    float spawnInterval = 1.0f;
    float elapsedTime = 0.f;

    // E VE NT
    while (windowRender.isOpen()) {
        sf::Event event;
        while (windowRender.pollEvent(event)) {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                windowRender.close();
        }

        // Handle player movement
        Motion direction;
        float speed = 500.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            direction = { {0.f, -1.f}, speed };
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            direction = { {0.f, 1.f}, speed };
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            direction = { {-1.f, 0.f}, speed };
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            direction = { {1.f, 0.f}, speed };
        }
        else {
            direction = { {0.f, 0.f}, 0.f };
        }

        update_position(playerEngine.PlayerTransform, direction, 0.016f);

        // S P A W N
        elapsedTime += spawnClock.restart().asSeconds();
        if (elapsedTime >= spawnInterval) {
            Enemy newBullet;

            Vec2 directionNormalized;
            newBullet.EnemyTransform.position = spacomp::randomizePosition(windowEngine, directionNormalized);

            if (newBullet.EnemyTransform.position.x >= 0 && newBullet.EnemyTransform.position.x <= windowEngine.size.x &&
                newBullet.EnemyTransform.position.y >= 0 && newBullet.EnemyTransform.position.y <= windowEngine.size.y) {

                newBullet.EnemyMotion.directionNormalized = directionNormalized;
                newBullet.EnemyMotion.speed = 50.0f;

                bulletsEngine.push_back(newBullet);
            }

            elapsedTime = 0.f;
        }

        // DES TRUC TION
        for (auto bullet = bulletsEngine.begin(); bullet != bulletsEngine.end();) {
            update_position(bullet->EnemyTransform, bullet->EnemyMotion, 0.016f);

            if (bullet->EnemyTransform.position.x < 0 || bullet->EnemyTransform.position.x > windowEngine.size.x ||
                bullet->EnemyTransform.position.y < 0 || bullet->EnemyTransform.position.y > windowEngine.size.y) {

                bullet = bulletsEngine.erase(bullet);
            }

            float bulletRadius = bulletRender.getRadius();
            if (circleIntersectsRectangle(bullet->EnemyTransform, bulletRadius, playerEngine.PlayerTransform)) {
                bullet = bulletsEngine.erase(bullet);
            }
            else {
                ++bullet;
            }
        }

        // REN DER
        windowRender.clear(sf::Color::Black);

        // P L A Y E R
        playerSprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
        playerSprite.setPosition(playerEngine.PlayerTransform.position.x, playerEngine.PlayerTransform.position.y);
        windowRender.draw(playerSprite);

        // B U L L E T S
        bulletRender.setFillColor(sf::Color(100, 250, 50));

        // B U L L E T
        for (auto& bullet : bulletsEngine) {
            bulletRender.setPosition(bullet.EnemyTransform.position.x, bullet.EnemyTransform.position.y);
            windowRender.draw(bulletRender);
        }

        windowRender.display();
    }

    return 0;
}
