#include "entity_manager.hpp"

#include <cassert>

#include "type.hpp"

namespace ecs
{
	EntityManager::EntityManager()
	{
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			mAvailableEntities.push(entity);
		}
	}

	Entity EntityManager::create_entity()
	{
		assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

		Entity id = mAvailableEntities.front();
		mAvailableEntities.pop();
		++mLivingEntityCount;

		return id;
	}

	void EntityManager::destroy_entity(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		mSignatures[entity].reset();
		mAvailableEntities.push(entity);
		--mLivingEntityCount;
	}

	void EntityManager::set_entity_signature(Entity entity, Signature signature)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");
		mSignatures[entity] = signature;
	}

	Signature EntityManager::get_entity_signature(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");
		return mSignatures[entity];
	}

	/*[[nodiscard]] EntityManager& entities()
	{
		static EntityManager s_singleton;
		return s_singleton;
	}*/
}
