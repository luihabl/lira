#pragma once

#include "../core/component.h"

#include <functional>
#include <vector>

namespace MicroNinja
{
	class MultiTimer : public Component
	{

	public:

		struct SingleTimer
		{
			float duration = 0.0f;
			std::function<void(MultiTimer*)> timeout = nullptr;
		};

		bool loop = false;

		MultiTimer() = default;
		MultiTimer(const std::vector<SingleTimer>& _timers);

		void update() override;

	private:

		size_t current_timer = 0;

		float total_duration = 0.0f;
		float counter = 0.0f;
		std::vector<SingleTimer> timers;

	};



}