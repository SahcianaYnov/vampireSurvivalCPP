#pragma once
#include <systems/movementComponent.hpp>

using namespace movcomp;

namespace playerent {
	struct Player
	{
		Transform PlayerTransform;
		Motion PlayerMotion;
	};
}