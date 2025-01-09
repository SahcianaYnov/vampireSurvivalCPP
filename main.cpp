#include <gtest/gtest.h>

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


void update_position(Transform &transform, const Motion &motion, float deltaTime)
{
	transform.position.x += motion.directionNormalized.x * motion.speed * deltaTime;
	transform.position.y += motion.directionNormalized.y * motion.speed * deltaTime;
}

TEST(VampireSurvivorTest, MovementTest)
{
	// AAA
	Transform origin = {
		.position = {0.f,0.f}
	};

	Motion movement = {
		.directionNormalized = {1.f,0.f},
		.speed = 10.0f
	};

	constexpr float deltaTime = 0.016f;

	Transform destination = origin;
	update_position(destination, movement, 0.016f);

	// Assert
	EXPECT_EQ(origin.position.x + movement.directionNormalized.x * movement.speed * deltaTime,
		destination.position.x);

	EXPECT_EQ(origin.position.y + movement.directionNormalized.y * movement.speed * deltaTime,
		destination.position.y);

}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}