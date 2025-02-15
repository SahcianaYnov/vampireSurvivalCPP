#pragma once

#include "ecs/type.hpp"

#include <unordered_map>
#include <set>
#include <iostream>

namespace ecs
{
	class __declspec(dllexport) System
	{
	protected:
		friend class SystemManager;
		std::set<Entity> mEntities{};
	};

	class __declspec(dllexport) SystemManager
	{
	private:
		std::unordered_map<std::string, Signature> mSignatures{};
		std::unordered_map<std::string, std::shared_ptr<System>> mSystems{};

	public:
		static SystemManager& singleton()
		{
			static SystemManager s_singleton;
			return s_singleton;
		}

		void register_system(std::string_view typeName, std::shared_ptr<System> system, Signature signature);

		void remove_entity(Entity entity);
		void update_entity_signature(Entity entity, Signature entitySignature);
	};
}
