#include "multitimer.h"
#include "../modules/game.h"

MicroNinja::MultiTimer::MultiTimer(const std::vector<SingleTimer>& _timers)
{
	timers = _timers;

	for (const auto& [dt, f] : timers)
		total_duration += dt;

	restart();
}

void MicroNinja::MultiTimer::update()
{
	if (counter >= 0.0f)
	{
		counter -= GameProperties::delta_time();

		if (counter <= 0.0f)
		{
			timers[current_timer].timeout(this);
			
			if (current_timer < timers.size() - 1)
				counter = timers[++current_timer].duration;
			else if (loop)
				restart();		
		}
	}
}

void MicroNinja::MultiTimer::restart(size_t index)
{
	current_timer = index;
	counter = timers[current_timer].duration;
}


