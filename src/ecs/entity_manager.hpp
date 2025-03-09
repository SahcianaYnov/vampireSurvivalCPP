#pragma once


#include <array>
#include <queue>
#include <cstddef>

#include "type.hpp"

namespace ecs { class EntityManager; }

namespace ecs
{
	[[nodiscard]] __declspec(dllexport) EntityManager& entities();

	class __declspec(dllexport) EntityManager
	{
	public:
		EntityManager();
		static EntityManager& singleton()
		{
			static EntityManager s_singleton;
			return s_singleton;
		}
		[[nodiscard]] Entity create_entity();
		void destroy_entity(Entity entity);
		void set_entity_signature(Entity entity, Signature signature);
		[[nodiscard]] Signature get_entity_signature(Entity entity);
	private:
		std::queue<Entity> mAvailableEntities{};
		std::array<Signature, MAX_ENTITIES> mSignatures{};
		size_t mLivingEntityCount{ 0u };
	};
}
