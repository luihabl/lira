#pragma once

#include <tinysdl.h>
#include "core/scene.h"

using namespace TinySDL;

namespace MicroNinja {

	class Level0 : public Scene {

		void begin() override;
		void update() override;
		void render(TinySDL::BatchRenderer& renderer) override;

		bool render_colliders = false;

	};
}