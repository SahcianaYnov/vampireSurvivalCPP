#include "bullet_system.hpp"
#include <ecs/component_manager.hpp>
#include <ecs/entity_manager.hpp>

#include "../../../../../.conan2/p/sfmld83ff95bb9185/p/include/SFML/Graphics/CircleShape.hpp"


bool circleIntersectsRectangle(const movcomp::Transform& circleTransform, float circleRadius, const movcomp::Transform& rectangleTransform) {
	float closestX = std::fmax(rectangleTransform.position.x, std::fmin(circleTransform.position.x, rectangleTransform.position.x + rectangleTransform.size.x));
	float closestY = std::fmax(rectangleTransform.position.y, std::fmin(circleTransform.position.y, rectangleTransform.position.y + rectangleTransform.size.y));

	float distanceX = circleTransform.position.x - closestX;
	float distanceY = circleTransform.position.y - closestY;
	float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

	return distance <= circleRadius;
}

namespace sf
{
	class CircleShape;
}

	void BulletSystem::updateBulletPositions(float deltaTime)
	{
		for (auto entity : mEntities) {
			auto& motion = ecs::ComponentManager::singleton().get_component<movcomp::Motion>(entity);
			auto& transform = ecs::ComponentManager::singleton().get_component<movcomp::Transform>(entity);
		}
	}

	void BulletSystem::deleteBullet(const movcomp::Transform& windowTransformData, const movcomp::Transform& playerTransformData)
{
	for (auto entity : mEntities) {
		updateBulletPositions(0.016f);

		auto& transform = ecs::ComponentManager::singleton().get_component<movcomp::Transform>(entity);
		auto& circleRender = ecs::ComponentManager::singleton().get_component<sf::CircleShape>(entity);

		sf::CircleShape radius = circleRender.getRadius();

		if ((transform.position.x < 0 || transform.position.x > windowTransformData.size.x ||
			transform.position.y < 0 || transform.position.y > windowTransformData.size.y) ||
			(circleIntersectsRectangle(transform, circleRender.getRadius(), playerTransformData))) {
			ecs::SystemManager::singleton().remove_entity<ecs::Entity>(entity);
		}
	}
}