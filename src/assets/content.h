#pragma once 

#include <tinysdl.h>
#include <string>
#include <filesystem> 

#include <unordered_map>

using namespace TinySDL;

namespace MicroNinja {

	class Content {

		std::string folder_name;
		std::filesystem::path find_path();

	public:
		void set_folder_name(const std::string& name);
		void load_all();
		
		static Texture* find_tex(const std::string& name);
		
	};
}