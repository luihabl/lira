#pragma once 

#include <tinysdl.h>
#include <string>
#include <filesystem> 

#include <unordered_map>

using namespace TinySDL;

namespace MicroNinja {

	class Content {

		template <typename T>
		static std::unordered_map<std::string, T> assets;

		std::string folder_name;
		std::filesystem::path find_path();

	public:
		void set_folder_name(const std::string& name);
		void load_all();
		
		static Texture* find_tex(const std::string& name);
		
		//template <typename T>
		//static T* find(const std::string& name);
	};

	//template<typename T>
	//std::unordered_map<std::string, T> Content::assets;
	//
	//template <typename T>
	//T* Content::find(const std::string& name) {
	//	return &(assets<T>.at(name));
	//}


}