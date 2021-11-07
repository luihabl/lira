#pragma once

#include <tinysdl.h>
#include "core/scene.h"
#include "assets/map.h"

using namespace TinySDL;

namespace MicroNinja {

	class Level : public Scene {

		void begin() override;
		void update() override;
		void render(TinySDL::BatchRenderer& renderer) override;

		bool render_colliders = false;
		bool render_minimap = false; // Move to component later

		IVec2 camera = IVec2::zeros;

		Map* current_map;
		std::string map_name;
		struct Room 
		{
			size_t id;
			IntRect bbox;
		};

		Room current_room;

		//size_t current_room_id;
		//IntRect current_room_bbox;

		std::vector<Room> rooms;

		int room_default_width = 320;
		int room_default_height = 184;

		void set_map(const std::string& map_name);
		int find_in_map(const std::string& name);
		void load_room(size_t id);
		void unload_room();

	};
}