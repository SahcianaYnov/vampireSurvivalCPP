#pragma once
#include "movementComponent.hpp"
#include "ecs/system_manager.hpp"

class __declspec(dllexport) BulletSystem : public ecs::System {
public:
	void updateBulletPositions(float deltaTime);
	void deleteBullet(const movcomp::Transform& windowTransformData, const movcomp::Transform& playerTransformData);
};
