#pragma once

#include "../core/ecs.h"
#include "collider.h"

#include <functional>

namespace Lira
{
	class Area : public Component
	{
	public:
		Collider* collider = nullptr;
		std::function<void(Area*)> on_overlap = nullptr;
		uint32_t overlapped_by = 0;

		void update() override
		{
			if (collider)
			{
				if (collider->check_first(overlapped_by))
				{
					if (on_overlap)
					{
						on_overlap(this);
					}
				}
			}
		}
	};
}