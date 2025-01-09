namespace movcomp {
	struct Vec2
	{
		float x;
		float y;
	};


	struct Transform
	{
		Vec2 position;
	};


	struct Motion
	{
		Vec2 directionNormalized;
		float speed;
	};

	void update_position(Transform& transform, const Motion& motion, float deltaTime);
}