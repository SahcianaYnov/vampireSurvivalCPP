#include "movementComponent.hpp"

namespace movcomp
{
	void update_position(Transform& transform, const Motion& motion, float deltaTime)
	{
		transform.position.x += motion.directionNormalized.x * motion.speed * deltaTime;
		transform.position.y += motion.directionNormalized.y * motion.speed * deltaTime;
	}

	[[nodiscard]] inline bool intersects(const Transform& base, const Transform& other)
	{
		return base.get_min_bound().x <= other.get_max_bound().x &&
			base.get_max_bound().x >= other.get_min_bound().x &&
			base.get_min_bound().y <= other.get_max_bound().y &&
			base.get_max_bound().y >= other.get_min_bound().y;
	}

	static bool isOutOfBounds(const Transform& base, const Transform& windowTransform) {
		return base.position.x < 0 || base.position.x > windowTransform.size.x ||
			base.position.y < 0 || base.position.y > windowTransform.size.y;
	}

}