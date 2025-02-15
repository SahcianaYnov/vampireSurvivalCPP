#pragma once

#include "ecs/type.hpp"

#include <array>
#include <queue>
#include <cstddef>

namespace ecs
{
	class __declspec(dllexport) EntityManager
	{
	private:
		std::queue<Entity> mAvailableEntities{};
		std::array<Signature, MAX_ENTITIES> mSignatures{};
		size_t mLivingEntityCount{0u};

	public:
		EntityManager();

		static EntityManager& singleton()
		{
			static EntityManager s_singleton;
			return s_singleton;
		}

		Entity create_entity();
		void destroy_entity(Entity entity);

		void set_entity_signature(Entity entity, Signature signature);
		Signature get_entity_signature(Entity entity);
	};
}
