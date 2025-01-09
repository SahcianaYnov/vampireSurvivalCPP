#include "movementComponent.hpp"

namespace movcomp
{
	void update_position(Transform& transform, const Motion& motion, float deltaTime)
	{
		transform.position.x += motion.directionNormalized.x * motion.speed * deltaTime;
		transform.position.y += motion.directionNormalized.y * motion.speed * deltaTime;
	}
}


