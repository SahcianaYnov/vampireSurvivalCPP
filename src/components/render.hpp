#pragma once

#include <SFML/Graphics.hpp>

struct SpriteRender
{
    static constexpr uint8_t NUM_FRAMES = 4;

    sf::Sprite sprite{};
    sf::Texture* texture = nullptr;
	uint8_t currentFrame = 0;
};
