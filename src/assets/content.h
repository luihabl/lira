#pragma once 

#include <tinysdl.h>
#include <string>
#include <filesystem> 

#include <unordered_map>

namespace MicroNinja {

	class Content {

		std::string folder_name;
		std::filesystem::path find_path();

	public:
		void set_folder_name(const std::string& name);
		void load_all();
		
		template <typename T>
		static T* find(const std::string& name);
	};
}