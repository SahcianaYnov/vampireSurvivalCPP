#pragma once

#include "components/base.hpp"
#include "ecs/system_manager.hpp"

#include <SFML/Graphics/Texture.hpp>

class EnemySystem : public ecs::System
{
public:
    void create_enemy(sf::Texture& spriteSheet, const Transform& screenTransform);
    void handle_movements(float deltaTime);
};
