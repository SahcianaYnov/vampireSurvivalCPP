#include "systems/enemy_system.hpp"

#include "components/base.hpp"
#include "components/render.hpp"

#include "ecs/entity_manager.hpp"
#include "ecs/component_manager.hpp"

void EnemySystem::create_enemy(sf::Texture& spriteSheet, const Transform& screenTransform)
{
    // Creation de l'entite pour le nouvel ennemi
    ecs::Entity enemyEntity = ecs::entities().create_entity();

    // On definit les composants et leur valeur de base pour l'ennemi
    // Chaque composant est associe à l'entite ennemi que l'on vient de creer
    Transform enemyTransform{
        .position = {screenTransform.size.x / 2.f, screenTransform.size.y / 2.f},
        .size = {16.f, 16.f},
        .scale = {2.f, 2.f}
    };
    ecs::components().add_component<Transform>(enemyEntity, std::move(enemyTransform));

    Motion enemyMotion{
        .direction = {0.f, 0.f},
        .acceleration = {10.f, 10.f}
    };
    ecs::components().add_component<Motion>(enemyEntity, std::move(enemyMotion));

    SpriteRender enemySpriteComp{};
    enemySpriteComp.texture = &spriteSheet;
    enemySpriteComp.sprite.setTexture(spriteSheet);
    enemySpriteComp.sprite.setScale(2.f, 2.f);
    ecs::components().add_component<SpriteRender>(enemyEntity, std::move(enemySpriteComp));

    Direction enemyDirection = Direction::down;
    ecs::components().add_component<Direction>(enemyEntity, std::move(enemyDirection));

    // On cree la signature de l'ennemi et on l'associe à l'entité
    ecs::Signature signature;
    signature.set(ecs::components().get_component_type<Transform>(), true);
    signature.set(ecs::components().get_component_type<Motion>(), true);
    signature.set(ecs::components().get_component_type<Direction>(), true);
    signature.set(ecs::components().get_component_type<SpriteRender>(), true);
    ecs::entities().set_entity_signature(enemyEntity, signature);

    // On met à jour l'ensemble des systèmes pour qu'ils puissent ajouter l'entité
    // Chaque système ajoute l'entité seulement si sa signature correspond à leur signature
    // (que les composants de la signature du système sont compris dans ceux de la signature de l'ennemi)
    ecs::systems().update_entity_signature(enemyEntity, signature);
}

void EnemySystem::handle_movements(float deltaTime)
{
    static constexpr float DEFAULT_SPEED = 10.f;

    // Parcourir toutes les entités gérées par EnemySystem
    for (ecs::Entity entity : entities())
    {
        // Récupération des composants Motion, Transform et Direction
        auto& motion = ecs::components().get_component<Motion>(entity);
        auto& transform = ecs::components().get_component<Transform>(entity);
        auto& direction = ecs::components().get_component<Direction>(entity);

        // 1 chance sur 4 de changer aléatoirement la direction de l’ennemi
        if (std::rand() % 4 == 0)
        {
            direction = static_cast<Direction>(std::rand() % 5);
        }

        // Mettre à jour le vecteur `motion.direction` en fonction de la direction de l’ennemi
        switch (direction)
        {
        case Direction::up:
            motion.direction = { 0.f, -DEFAULT_SPEED };
            break;
        case Direction::down:
            motion.direction = { 0.f, DEFAULT_SPEED };
            break;
        case Direction::left:
            motion.direction = { -DEFAULT_SPEED, 0.f };
            break;
        case Direction::right:
            motion.direction = { DEFAULT_SPEED, 0.f };
            break;
        case Direction::none:
            motion.direction = { 0.f, 0.f };
            break;
        }

        // Mise à jour de la position selon la direction et l'accélération
        transform.position.x += motion.acceleration.x * motion.direction.x * deltaTime;
        transform.position.y += motion.acceleration.y * motion.direction.y * deltaTime;
    }
}
