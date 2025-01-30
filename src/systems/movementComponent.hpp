#pragma once

namespace movcomp {
	struct Vec2
	{
		float x;
		float y;
	};

	struct Transform
	{
		Vec2 position;
		Vec2 size{ .x = 1.0f, .y = 1.0f };

		// HAUT GAUCHE
		[[nodiscard]] Vec2 get_min_bound() const
		{
			return position;
		}

		// POSITIF X HAUT DROIT
		// POSITIF Y BAS GAUCHE

		//BAS DROIT
		[[nodiscard]] Vec2 get_max_bound() const
		{
			return { .x = position.x + size.x, .y = position.y + size.y };
		}

		// NEGATIF Y HAUT DROIT
		// NEGATIF X BAS GAUCHE
	};

	struct Motion
	{
		Vec2 directionNormalized;
		float speed;
	};

	[[nodiscard]] inline bool intersects(const Transform& base, const Transform& other);

	void update_position(Transform& transform, const Motion& motion, float deltaTime);

	static bool isOutOfBounds(const Transform& base, const Transform& windowTransform);
}