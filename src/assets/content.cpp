#include "content.h"

#include <tinysdl.h>
#include "ldtk.h"

#include <iostream>
#include <unordered_map>
#include <filesystem> 

using namespace TinySDL;
using namespace MicroNinja;

namespace fs = std::filesystem;

namespace {

	template <typename T>
	std::unordered_map<std::string, T> assets;

	template <typename T>
	void set_asset(std::string key, const T & asset) {
		assets<T>[key] = asset;
	}

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
				set_asset<Texture>(key_name, Texture::from_file(item.path().generic_string().c_str()));
			}

			if (extension == ".ldtk") {
				set_asset<LDTk::File>(key_name, nlohmann::json::parse(File::load_txt(item.path().generic_string().c_str())));
			}
				

		}
	}
}


template <typename T>
T* Content::find(const std::string& name) {
	return &(assets<T>[name]);
}

template Texture* Content::find<Texture>(const std::string& name);
template LDTk::File* Content::find<LDTk::File>(const std::string& name);

