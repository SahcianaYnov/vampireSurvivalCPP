#pragma once

#include <bitset>

namespace ecs
{
	using Entity = uint32_t;
	constexpr Entity MAX_ENTITIES = 100'000u;

	using Component = uint8_t;
	constexpr Component MAX_COMPONENTS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;
}
