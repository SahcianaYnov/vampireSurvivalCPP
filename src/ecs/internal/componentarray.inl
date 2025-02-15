#include <cassert>

#include "ecs/type.hpp"

#include <vector>
#include <unordered_map>

namespace ecs::details
{
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void remove_entitycomponent(Entity entity) = 0;
	};

	template<typename TComponent>
	class ComponentArray : public IComponentArray
	{
	private:
		std::vector<TComponent> mComponents{};

		std::unordered_map<Entity, size_t> mEntityToComponentIndex{};
		std::unordered_map<size_t, Entity> mComponentIndexToEntity{};

		void add_entity(Entity entity, size_t componentIndex)
		{
			mEntityToComponentIndex[entity] = componentIndex;
			mComponentIndexToEntity[componentIndex] = entity;
		}

	public:
		ComponentArray()
		{
			mComponents.reserve(MAX_ENTITIES);
		}

		void insert_entitycomponent(Entity entity, TComponent&& component)
		{
			size_t newComponentIndex = mComponents.size();
			mComponents.push_back(std::move(component));

			mEntityToComponentIndex[entity] = newComponentIndex;
			mComponentIndexToEntity[newComponentIndex] = entity;
		}

		void remove_entitycomponent(Entity entity) override
		{
			if (mEntityToComponentIndex.find(entity) == mEntityToComponentIndex.end())
				return;

			size_t indexOfRemovedEntity = mEntityToComponentIndex[entity];
			size_t indexOfLastEntity = mComponents.size() - 1;

			// Swap the removed entity with the last element and remove it
			std::swap(mComponents[indexOfRemovedEntity], mComponents[indexOfLastEntity]);
			mComponents.pop_back();

			// Update the mapping of the last entity to the removed slot
			Entity lastEntity = mComponentIndexToEntity[indexOfLastEntity];
			mEntityToComponentIndex[lastEntity] = indexOfRemovedEntity;
			mComponentIndexToEntity[indexOfRemovedEntity] = lastEntity;
			mEntityToComponentIndex.erase(entity);
			mComponentIndexToEntity.erase(indexOfLastEntity);
		}

		TComponent& get_entitycomponent(Entity entity)
		{
			assert(mEntityToComponentIndex.find(entity) != mEntityToComponentIndex.end() &&
				"Trying to retrieve a non-existent component.");

			size_t indexOfEntity = mEntityToComponentIndex[entity];
			return mComponents[indexOfEntity];
		}
	};
}
