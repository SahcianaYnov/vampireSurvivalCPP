#include "systems/enemy_system.hpp"
#include "components/base.hpp"
#include "components/render.hpp"
#include "ecs/entity_manager.hpp"
#include "ecs/component_manager.hpp"

void EnemySystem::create_enemy(sf::Texture& spriteSheet, Transform& enemyTransform)
{
    auto& componentManager = ecs::ComponentManager::singleton();
    auto& entityManager = ecs::EntityManager::singleton();
    auto& systemManager = ecs::SystemManager::singleton();

    ecs::Entity enemyEntity = entityManager.create_entity();

    Motion enemyMotion{.acceleration = {1.f, 1.f}};
    componentManager.add_component<Motion>(enemyEntity, std::move(enemyMotion));

    SpriteRender enemySpriteComp{};
    enemySpriteComp.texture = &spriteSheet;
    enemySpriteComp.sprite.setTexture(spriteSheet);
    enemySpriteComp.sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    enemySpriteComp.sprite.setPosition(enemyTransform.position.x, enemyTransform.position.y);


    componentManager.add_component<Transform>(enemyEntity, std::move(enemyTransform));


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

void EnemySystem::handle_movements(float deltaTime, Vector2& goal)
{
    static constexpr float DEFAULT_SPEED = 50.f; // Augmente un peu la vitesse pour tester

    auto& componentManager = ecs::ComponentManager::singleton();

    for (ecs::Entity entity : entities())
    {
        auto& motion = componentManager.get_component<Motion>(entity);
        auto& transform = componentManager.get_component<Transform>(entity);

        // Calculer le vecteur direction vers le joueur
        Vector2 direction = {
            goal.x - transform.position.x,
            goal.y - transform.position.y
        };

        // Normaliser la direction
        float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (magnitude > 0)
        {
            direction.x /= magnitude;
            direction.y /= magnitude;
        }

        // Appliquer la vitesse et le déplacement
        transform.position.x += direction.x * DEFAULT_SPEED * deltaTime;
        transform.position.y += direction.y * DEFAULT_SPEED * deltaTime;
    }
}

bool EnemySystem::isOutOfBounds(const Transform& base, const Transform& windowTransform) {
    return base.position.x < 0 || base.position.x > windowTransform.size.x ||
        base.position.y < 0 || base.position.y > windowTransform.size.y;
}

bool EnemySystem::intersects(const Transform& enemy, const Transform& player) {
    return enemy.position.x < player.position.x + player.size.x &&
        enemy.position.x + enemy.size.x > player.position.x &&
        enemy.position.y < player.position.y + player.size.y &&
        enemy.position.y + enemy.size.y > player.position.y;
}

void EnemySystem::checkCollisions(const Transform& collider) {
    auto& componentManager = ecs::ComponentManager::singleton();
    std::vector<ecs::Entity> enemiesToRemove;

    for (auto entity : entities()) {
        auto& enemyTransform = componentManager.get_component<Transform>(entity);

        if (intersects(enemyTransform, collider)) {
            enemiesToRemove.push_back(entity);
        }
    }

    for (auto entity : enemiesToRemove) {
        ecs::SystemManager::singleton().remove_entity(entity);
        ecs::EntityManager::singleton().destroy_entity(entity);
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