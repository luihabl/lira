#pragma once

#include "../core/ecs.h"
#include "collider.h"

#include <functional>

namespace Lira
{
	class Hittable : public Component
	{
	public:
		Collider* collider = nullptr;
		std::function<void(Hittable*)> on_hit = nullptr;
		uint32_t hit_by = 0;

		void update() override
		{
			if (collider)
			{
				if (collider->check_first(hit_by))
				{
					if (on_hit)
					{
						on_hit(this);
					}
				}
			}
		}
	};
}