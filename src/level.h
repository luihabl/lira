#pragma once

#include <tinysdl.h>
#include "core/ecs.h"
#include "assets/map.h"
#include "layers.h"

using namespace TinySDL;

namespace Lira {

	class Level : public Scene {

		public:

		void begin() override;
		void update() override;
		void render(TinySDL::BatchRenderer& renderer) override;
		void end() override;

		void layer_transform(Layer::Draw layer, const TinySDL::Mat3x2& transform);

		bool render_colliders = false;

		IVec2 camera = IVec2::zeros;

		Entity* minimap = nullptr;

		Map* current_map;
		std::string map_name;
		struct Room 
		{
			size_t id;
			IntRect bbox;
		};

		Room current_room;

		std::vector<Room> rooms;

		int room_default_width = 320;
		int room_default_height = 184;

		void set_map(const std::string& map_name);
		int find_in_map(const std::string& name);
		void load_room(size_t id);
		void unload_room();
		void move_to_room(size_t id);

	};
}