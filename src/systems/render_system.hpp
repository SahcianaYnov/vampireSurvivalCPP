#pragma once

#include <SFML/Graphics.hpp>
#include "ecs/system_manager.hpp"

class SpriteRenderSystem : public ecs::System
{
public:
    // Updates sprite animations (e.g. frames) for all entities with SpriteRender
    void updateSpriteAnimation(float deltaTime);

    // Renders all visible entities (player, tilemap, etc.)
    void render(sf::RenderWindow& window);
};
