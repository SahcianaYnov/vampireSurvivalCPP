#pragma once

#include "components/base.hpp"
#include "ecs/system_manager.hpp"
#include <SFML/Graphics.hpp>
#include "movementComponent.hpp"

class __declspec(dllexport) BulletSystem : public ecs::System {
public:

	void update(float deltaTime);

	bool circleIntersectsRectangle(const Transform &circleTransform, float circleRadius, const Transform &rectangleTransform);

	bool isOutOfBounds(const Transform &base, const Transform &windowTransform);

	void deleteBullet(ecs::Entity entity);

	void create_bullet(Transform &bulletTransform, Motion &bulletMotion);

	void render_bullet(sf::RenderWindow &window);
	void checkCollisions(Transform& bulletTransform, Transform& windowTransform, Transform& playerTransform, float circleRadius);
};
