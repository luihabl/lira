#pragma once

#include "../core/ecs.h"
#include "collider.h"

#include <functional>

namespace Lira
{
	class Interactable : public Component
	{
	public:
		Collider* collider = nullptr;
		std::function<void(Interactable*)> on_interact = nullptr;
		uint32_t interacted_by = 0;
		bool can_interact = true;

		void update() override
		{
			if (collider && can_interact)
			{
				if (collider->check_first(interacted_by))
				{
					if (on_interact)
					{
						on_interact(this);
					}
				}
			}
		}
	};
}