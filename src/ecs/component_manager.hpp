#pragma once

#include "ecs/type.hpp"
#include "ecs/internal/componentarray.inl"

#include <array>
#include <cassert>
#include <unordered_map>

namespace ecs { class ComponentManager; }

namespace ecs
{
	[[nodiscard]] __declspec(dllexport) ComponentManager& components();

	class __declspec(dllexport) ComponentManager
	{
	public:
		template<typename TComponent>
		void register_component();

		template<typename TComponent>
		void add_component(Entity entity, TComponent&& component);

		template<typename TComponent>
		void remove_component(Entity entity);

		template<typename TComponent>
		TComponent& get_component(Entity entity);

		static ComponentManager& singleton()
		{
			static ComponentManager s_singleton;
			return s_singleton;
		}

		void remove_entity(Entity entity, const Signature& signature);

		template<typename TComponent>
		Component get_component_type();
	private:
		std::array<std::unique_ptr<details::IComponentArray>, MAX_COMPONENTS> mComponentArrays{};
		static inline Component sComponentTypeIndex = 0;

		template<typename TComponent>
		Component get_typeindex();

		template<typename TComponent>
		details::ComponentArray<TComponent>* get_componentarray(Entity entity);
	};

	template<typename TComponent>
	void ComponentManager::register_component()
	{
		Component typeId = get_typeindex<TComponent>();
		assert(typeId < MAX_COMPONENTS && "Too many component types registered!");
		assert(mComponentArrays[typeId] == nullptr && "Registering component type more than once.");
		mComponentArrays[typeId] = std::make_unique<details::ComponentArray<TComponent>>();
	}

	template<typename TComponent>
	Component ComponentManager::get_component_type()
	{
		return get_typeindex<TComponent>();
	}

	template<typename TComponent>
	void ComponentManager::add_component(Entity entity, TComponent&& component)
	{
		get_componentarray<TComponent>(entity)->insert_entitycomponent(entity, std::move(component));
	}

	template<typename TComponent>
	void ComponentManager::remove_component(Entity entity)
	{
		get_componentarray<TComponent>(entity)->remove_entitycomponent(entity);
	}

	template<typename TComponent>
	TComponent& ComponentManager::get_component(Entity entity)
	{
		return get_componentarray<TComponent>(entity)->get_entitycomponent(entity);
	}

	inline void ComponentManager::remove_entity(Entity entity, const Signature& signature)
	{
		for (Component i = 0; i < MAX_COMPONENTS; i++)
		{
			if (signature.test(i) && mComponentArrays[i] != nullptr)
			{
				mComponentArrays[i]->remove_entitycomponent(entity);
			}
		}
	}

	template<typename TComponent>
	Component ComponentManager::get_typeindex()
	{
		static const Component typeId = sComponentTypeIndex++;
		return typeId;
	}

	template<typename TComponent>
	details::ComponentArray<TComponent>* ComponentManager::get_componentarray(Entity entity)
	{
		Component typeId = get_typeindex<TComponent>();
		assert(mComponentArrays[typeId] != nullptr && "Component not registered.");
		return static_cast<details::ComponentArray<TComponent>*>(mComponentArrays[typeId].get());
	}

	[[nodiscard]] inline ComponentManager& components()
	{
		static ComponentManager s_singleton;
		return s_singleton;
	}
}
