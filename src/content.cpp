#include <tinysdl.h>
#include <unordered_map>
#include <filesystem> 
#include <any>

#include "content.h"

#include <iostream>

using namespace TinySDL;
using namespace MicroNinja;

namespace fs = std::filesystem;

namespace {
	std::unordered_map<std::string, Texture> textures;

	//template <typename T>
	//void add_file(std::unordered_map<std::string, T>& assets, const std::string & key, std::string path) {
	//	assets[key] = T::from_file(path.c_str());
	//}

}

void Content::set_folder_name(const std::string & name) {
	folder_name = name;
}

// This implementation has to change afterwards since it depends
// on the location where the executable is being run.
fs::path Content::find_path() {
	auto exe_path = fs::current_path();
	auto content_path = fs::path(folder_name);

	for (int i = 0; i < 10; i++) {

		if (std::filesystem::is_directory(exe_path / content_path)) {
			return (exe_path / content_path);
		}

		content_path = ".." / content_path;
	}
	Log::warn("Content folder '%s' was not found.", folder_name.c_str());

	return "";
}

void Content::load_all() {

	auto content_path = find_path();
	for (auto& item : fs::recursive_directory_iterator(content_path)) {
		if (item.is_regular_file()) {
			fs::path file_path = fs::relative(item.path(), content_path);
			std::string extension = file_path.extension().generic_string();
			file_path.replace_extension("");
			
			std::string key_name = file_path.generic_string();
			std::replace(key_name.begin(), key_name.end(), '\\', '/');

			
			if (extension == ".png") {
				textures[key_name] = Texture::from_file(item.path().generic_string().c_str());
				//assets<Texture>[key_name] = Texture::from_file(item.path().generic_string().c_str());
				//add_file(assets<Texture>, key_name, item.path().generic_string());
			}
				

		}
	}
}

Texture* Content::find_tex(const std::string& name) {
	return &(textures.at(name));
}

