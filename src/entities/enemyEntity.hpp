#pragma once
#include <systems/movementComponent.hpp>

using namespace movcomp;

namespace enemyent {
	struct Enemy
	{
		movcomp::Vec2 EnemyPosition;
		movcomp::Transform EnemyTransform;
		movcomp::Motion EnemyMotion;
	};

}