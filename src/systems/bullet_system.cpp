#include "bullet_system.hpp"
#include <components/render.hpp>
#include "spawnComponent.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/entity_manager.hpp"

void BulletSystem::update(float deltaTime) {
    auto& componentManager = ecs::ComponentManager::singleton();

    for (auto entity : entities()) {
        auto& motion = componentManager.get_component<Motion>(entity);
        auto& transform = componentManager.get_component<Transform>(entity);

        transform.position.x += motion.direction.x * motion.acceleration.x * deltaTime;
        transform.position.y += motion.direction.y * motion.acceleration.y * deltaTime;
    }
}


bool BulletSystem::circleIntersectsRectangle(const Transform& circleTransform, float circleRadius, const Transform& rectangleTransform) {
    float closestX = std::fmax(rectangleTransform.position.x, std::fmin(circleTransform.position.x, rectangleTransform.position.x + rectangleTransform.size.x));
    float closestY = std::fmax(rectangleTransform.position.y, std::fmin(circleTransform.position.y, rectangleTransform.position.y + rectangleTransform.size.y));

    float distanceX = circleTransform.position.x - closestX;
    float distanceY = circleTransform.position.y - closestY;
    float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

    return distance <= circleRadius;
}

bool BulletSystem::isOutOfBounds(const Transform& base, const Transform& windowTransform) {
    return base.position.x < 0 || base.position.x > windowTransform.size.x ||
           base.position.y < 0 || base.position.y > windowTransform.size.y;
}

void BulletSystem::deleteBullet(ecs::Entity entity) {
	ecs::SystemManager::singleton().remove_entity(entity);
	ecs::EntityManager::singleton().destroy_entity(entity);
}

void BulletSystem::create_bullet(Transform& bulletTransform, Motion& bulletMotion) {
    auto& componentManager = ecs::ComponentManager::singleton();
    auto& entityManager = ecs::EntityManager::singleton();
    auto& systemManager = ecs::SystemManager::singleton();

    ecs::Signature signature;
    ecs::Entity bulletEntity = entityManager.create_entity();

    sf::CircleShape shape(10.f);
    shape.setFillColor(sf::Color::Red);

    componentManager.add_component<sf::CircleShape>(bulletEntity, std::move(shape));
    componentManager.add_component<Transform>(bulletEntity, std::move(bulletTransform));
    componentManager.add_component<Motion>(bulletEntity, std::move(bulletMotion));

    signature.set(componentManager.get_component_type<Transform>(), true);
    signature.set(componentManager.get_component_type<Motion>(), true);
    signature.set(componentManager.get_component_type<sf::CircleShape>(), true);

    entityManager.set_entity_signature(bulletEntity, signature);
	systemManager.update_entity_signature(bulletEntity, signature);
}

void BulletSystem::render_bullet(sf::RenderWindow& window) {
    for (auto entity : entities()) {
        {
            auto& transform = ecs::ComponentManager::singleton().get_component<Transform>(entity);
            auto& circle = ecs::ComponentManager::singleton().get_component<sf::CircleShape>(entity);

            circle.setPosition(transform.position.x, transform.position.y);
            window.draw(circle);

        }
    }
}

void BulletSystem::checkCollisions(const Transform& windowTransform, const Transform& playerTransform) {
    auto& componentManager = ecs::ComponentManager::singleton();
    std::vector<ecs::Entity> bulletsToRemove;

    for (auto entity : entities()) {
        auto& bulletTransform = componentManager.get_component<Transform>(entity);
        auto& circle = ecs::ComponentManager::singleton().get_component<sf::CircleShape>(entity);

        if (isOutOfBounds(bulletTransform, windowTransform) || circleIntersectsRectangle(bulletTransform, circle.getRadius(), playerTransform)) {
            bulletsToRemove.push_back(entity);
        }
    }

    for (auto entity : bulletsToRemove) {
        deleteBullet(entity);
    }
}

