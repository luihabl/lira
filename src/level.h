#pragma once

#include <tinysdl.h>
#include "core/ecs.h"
#include "assets/map.h"
#include "layers.h"
#include "inventory.h"

using namespace TinySDL;

namespace Lira {

	class Level : public Scene {

		public:

		void begin() override;
		void update() override;
		void render(TinySDL::BatchRenderer& renderer) override;
		void end() override;

		void layer_transform(Layer::Draw layer, const TinySDL::Mat3x2& transform);

		Vec2 camera = Vec2::zeros;
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
		float transition_easing = 0.0f;
		Room next_room;

		std::unordered_map<size_t, std::vector<std::string>> persistent_interactions;
		void add_persistent_interaction(const std::string& id);
		void clear_persistent_interactions();
		bool has_persistent_interaction(size_t room_id, const std::string& id);

		Inventory inventory;

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