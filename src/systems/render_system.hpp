#pragma once
#include "ecs/system_manager.hpp"


class __declspec(dllexport) RenderSystem : public ecs::System {
public:
	void render();
};