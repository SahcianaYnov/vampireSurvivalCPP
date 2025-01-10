#pragma once
#include <systems/movementComponent.hpp>

using namespace movcomp;

namespace enemyent {
	struct Enemy
	{
		Transform EnemyTransform;
		Motion EnemyMotion;
	};

}