#pragma once

#include "ecs/type.hpp"

#include <unordered_map>
#include <set>
#include <iostream>

namespace ecs { class SystemManager; }

namespace ecs
{
	[[nodiscard]] __declspec(dllexport) SystemManager& systems();

	class __declspec(dllexport) System
	{
	protected:
		std::set<Entity>& entities() { return mEntities; }
	private:
		friend class SystemManager;
		std::set<Entity> mEntities{};
	};

	class __declspec(dllexport) SystemManager
	{
	public:
		void register_system(std::string_view typeName, std::shared_ptr<System> system, Signature signature);

		template<typename TSystem>
		[[nodiscard]] std::shared_ptr<TSystem> get_system(const std::string& typeName);

		void remove_entity(Entity entity);
		void update_entity_signature(Entity entity, Signature entitySignature);

	private:
		std::unordered_map<std::string, Signature> mSignatures{};
		std::unordered_map<std::string, std::shared_ptr<System>> mSystems{};

	};

	template<typename TSystem>
	[[nodiscard]] std::shared_ptr<TSystem> SystemManager::get_system(const std::string& typeName)
	{
		return mSystems[typeName];
	}
}
