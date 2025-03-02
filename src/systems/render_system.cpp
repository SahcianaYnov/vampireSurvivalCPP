#include "systems/render_system.hpp"

#include "ecs/component_manager.hpp"
#include "components/base.hpp"
#include "components/render.hpp"

namespace
{
    // Cette fonction est propre à mon spritesheet (qui contient beaucoup de frames d'animation différentes)
    // Cette fonction n'est pas importante à regarder même si vous pouvez vous en inspirez si vous le souhaitez
    // (le code est vraiment moyen mais fonctionnel)
    void setSpriteTexture(SpriteRender& spriteRender, int frame, Direction direction)
    {
        static constexpr int FRAME_WIDTH = 32;
        static constexpr int FRAME_HEIGHT = 32;

        int texPosX = frame % SpriteRender::NUM_FRAMES;

        int texPosY = 0;
        switch (direction)
        {
        case Direction::down:
            texPosY = 6;
            break;
        case Direction::left:
            texPosY = 8;
            break;
        case Direction::right:
            texPosY = 10;
            break;
        case Direction::up:
            texPosY = 12;
            break;
        case Direction::none:
            texPosY = 0;
            break;
        }

        sf::IntRect rect;
        rect.left = texPosX * FRAME_WIDTH;
        rect.top = texPosY * FRAME_HEIGHT;
        rect.width = FRAME_WIDTH;
        rect.height = FRAME_HEIGHT;

        spriteRender.sprite.setTextureRect(rect);
    }
}

void SpriteRenderSystem::updateSpriteAnimation(float deltaTime)
{
    // Parcours de toutes les entités dont la signature correspond au système de rendu de sprite
    for (auto entity : entities())
    {
        // Récupération des composants requis
        auto& spriteRender = ecs::components().get_component<SpriteRender>(entity);
        auto& direction = ecs::components().get_component<Direction>(entity);

        // Passage à la frame d’animation suivante
    	spriteRender.currentFrame =
                (spriteRender.currentFrame + 1) % SpriteRender::NUM_FRAMES;

        // Mise à jour du sprite pour refléter la frame et la direction
        setSpriteTexture(spriteRender, spriteRender.currentFrame, direction);
    }
}

void SpriteRenderSystem::render(sf::RenderWindow& window)
{
    // Parcours de toutes les entités dont la signature correspond au système de rendu de sprite
    for (auto entity : entities())
    {
        // Récupération des composants requis
        auto& spriteRender = ecs::components().get_component<SpriteRender>(entity);
        auto& transform = ecs::components().get_component<Transform>(entity);

        // On place le sprite (rendu) à la position et à l’échelle de l’entité (dans le moteur)
        spriteRender.sprite.setPosition(transform.position.x, transform.position.y);
        spriteRender.sprite.setScale(transform.scale.x, transform.scale.y);

        // Dessin du sprite dans la fenêtre
        window.draw(spriteRender.sprite);
    }
}
