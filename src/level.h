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

		IVec2 camera = IVec2::zeros;
		std::vector<Layer::Draw> camera_layers;

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

		std::vector<Entity*> last_room_entities;
		bool render_colliders = false;
		bool room_transition = false;
		Room next_room;

		int room_default_width = 320;
		int room_default_height = 184;

		void set_map(const std::string& map_name);
		int find_in_map(const std::string& name);
		void load_room(size_t id);
		void unload_room();
		void move_to_room(size_t id);
		IVec2 room_camera_target(const TinySDL::IVec2& player_pos, const Room& room);

	};
}