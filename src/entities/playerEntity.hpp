#pragma once
#include <systems/movementComponent.hpp>

using namespace movcomp;

namespace playerent {
	struct Player {
		movcomp::Vec2 PlayerPosition;
		movcomp::Transform PlayerTransform;
		movcomp::Motion PlayerMotion;
	};
}