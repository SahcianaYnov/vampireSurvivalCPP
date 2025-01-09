#include <gtest/gtest.h>

#include "movementComponent.hpp"

using namespace movcomp;

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