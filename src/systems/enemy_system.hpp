#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "components/base.hpp"
#include "ecs/system_manager.hpp"

class EnemySystem : public ecs::System
{
public:
    void create_enemy(sf::Texture& spriteSheet, Transform& enemyTransform);
    void handle_movements(float deltaTime, Vector2& goal);
    bool isOutOfBounds(const Transform& base, const Transform& windowTransform);
    bool intersects(const Transform& base, const Transform& other);
    void checkCollisions(const Transform& collider);
    Transform randomizePosition(const Transform& windowSize, Vector2& playerPosition);
    void render_enemy(sf::RenderWindow& window);

private:
    size_t enemyCount = 0;

};
