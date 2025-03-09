#include "systems/enemy_system.hpp"

#include "movementComponent.hpp"
#include "components/base.hpp"
#include "components/render.hpp"

#include "ecs/entity_manager.hpp"
#include "ecs/component_manager.hpp"

void EnemySystem::create_enemy(sf::Texture& spriteSheet, const Transform& screenTransform)
{

    auto& componentManager = ecs::ComponentManager::singleton();
    auto& entityManager = ecs::EntityManager::singleton();
    auto& systemManager = ecs::SystemManager::singleton();

    ecs::Entity enemyEntity = entityManager.create_entity();

    Transform enemyTransform{
        .position = {screenTransform.size.x / 2.f, screenTransform.size.y / 2.f},
        .size = {16.f, 16.f},
        .scale = {2.f, 2.f}
    };
    componentManager.add_component<Transform>(enemyEntity, std::move(enemyTransform));

    Motion enemyMotion{
       .direction = {0.f, 0.f},
       .acceleration = {10.f, 10.f}
    };
    componentManager.add_component<Motion>(enemyEntity, std::move(enemyMotion));

    SpriteRender enemySpriteComp{};
    enemySpriteComp.texture = &spriteSheet;
    enemySpriteComp.sprite.setTexture(spriteSheet);
    enemySpriteComp.sprite.setScale(2.f, 2.f);
    componentManager.add_component<SpriteRender>(enemyEntity, std::move(enemySpriteComp));

    Direction enemyDirection = Direction::down;
    componentManager.add_component<Direction>(enemyEntity, std::move(enemyDirection));

    // On cree la signature de l'ennemi et on l'associe à l'entité
    ecs::Signature signature;
    signature.set(componentManager.get_component_type<Transform>(), true);
    signature.set(componentManager.get_component_type<Motion>(), true);
    signature.set(componentManager.get_component_type<Direction>(), true);
    signature.set(componentManager.get_component_type<SpriteRender>(), true);
    entityManager.set_entity_signature(enemyEntity, signature);

    // On met à jour l'ensemble des systèmes pour qu'ils puissent ajouter l'entité
    // Chaque système ajoute l'entité seulement si sa signature correspond à leur signature
    // (que les composants de la signature du système sont compris dans ceux de la signature de l'ennemi)
    systemManager.update_entity_signature(enemyEntity, signature);
    
}


void EnemySystem::handle_movements(float deltaTime)
{
    static constexpr float DEFAULT_SPEED = 10.f;

    auto& componentManager = ecs::ComponentManager::singleton();

    // Parcourir toutes les entités gérées par EnemySystem
    for (ecs::Entity entity : entities())
    {
        // Récupération des composants Motion, Transform et Direction
        auto& motion = componentManager.get_component<Motion>(entity);
        auto& transform = componentManager.get_component<Transform>(entity);
        auto& direction = componentManager.get_component<Direction>(entity);

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

bool EnemySystem::isOutOfBounds(const Transform& base, const Transform& windowTransform) {
    return base.position.x < 0 || base.position.x > windowTransform.size.x ||
        base.position.y < 0 || base.position.y > windowTransform.size.y;
}

bool EnemySystem::intersects(const Transform& base, const Transform& other)
{
    return base.get_min_bound().x <= other.get_max_bound().x &&
        base.get_max_bound().x >= other.get_min_bound().x &&
        base.get_min_bound().y <= other.get_max_bound().y &&
        base.get_max_bound().y >= other.get_min_bound().y;
}

void EnemySystem::checkCollisions(const Transform& screenTransform, const Transform& collider)
{
    for (ecs::Entity entity : entities())
    {
        auto& transform = ecs::ComponentManager::singleton().get_component<Transform>(entity);

	    if (isOutOfBounds(transform, screenTransform) || intersects(transform, collider))
	    {
            ecs::SystemManager::singleton().remove_entity(entity);
            ecs::EntityManager::singleton().destroy_entity(entity);
	    }
    }

}


Transform EnemySystem::randomizePosition(const Transform& windowSize, Vector2& playerPosition)
{
    static constexpr float DEFAULT_SPEED = 10.f;

    Transform newTransform;
    Vector2 maxBound = windowSize.get_max_bound();
    int zone = rand() % 4;

    // Déterminer la position aléatoire sur un des bords
    switch (zone)
    {
    case 0: // Haut
        newTransform.position = { static_cast<float>(rand() % static_cast<int>(maxBound.x)), 0.f };
        break;
    case 1: // Bas
        newTransform.position = { static_cast<float>(rand() % static_cast<int>(maxBound.x)), maxBound.y };
        break;
    case 2: // Gauche
        newTransform.position = { 0.f, static_cast<float>(rand() % static_cast<int>(maxBound.y)) };
        break;
    case 3: // Droite
        newTransform.position = { maxBound.x, static_cast<float>(rand() % static_cast<int>(maxBound.y)) };
        break;
    }

    // Calculer la direction vers le joueur
    Vector2 direction = { playerPosition.x - newTransform.position.x, playerPosition.y - newTransform.position.y };

    // Normaliser la direction
    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (magnitude > 0)
    {
        direction.x /= magnitude;
        direction.y /= magnitude;
    }

    // Appliquer la direction normalisée au Transform
    newTransform.scale = { direction.x, direction.y }; // Peut-être à changer selon ton système

    return newTransform;
}

void EnemySystem::render_enemy(sf::RenderWindow& window) {
    for (auto& enemy : entities()) {
        auto& enemyTransform = ecs::ComponentManager::singleton().get_component<Transform>(enemy);
        auto& enemySprite = ecs::ComponentManager::singleton().get_component<SpriteRender>(enemy);

        enemySprite.sprite.setPosition(enemyTransform.position.x, enemyTransform.position.y);

        window.draw(enemySprite.sprite);
    }
}