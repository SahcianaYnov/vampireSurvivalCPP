#include "ecs/system_manager.hpp"

namespace ecs
{
	void SystemManager::register_system(std::string_view typeName, std::shared_ptr<System> system, Signature signature)
	{
		mSystems.emplace(std::make_pair(typeName, std::move(system)));
		mSignatures.emplace(std::make_pair(typeName, signature));
	}

	void SystemManager::remove_entity(Entity entity)
	{
		for (const auto& [_, system] : mSystems)
		{
			system->mEntities.erase(entity);
		}
	}

	void SystemManager::update_entity_signature(Entity entity, Signature entitySignature)
	{
		for (const auto& [type, system] : mSystems)
		{
			auto const& systemSignature = mSignatures[type];

			if (const bool systemUsesEntity = ((entitySignature & systemSignature) == systemSignature))
			{
				system->mEntities.insert(entity);
			}
			else
			{
				system->mEntities.erase(entity);
			}
		}
	}
}
