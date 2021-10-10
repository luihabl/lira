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


		IVec2 camera_offset = IVec2::zeros;

		std::string current_map_name;
		size_t current_room_id;

		IntRect current_room_bbox;


		std::vector<IntRect> bbox_rooms;

		void set_map_info(const std::string& map_name);
		void load_room(size_t id);


	};
}