#include "content.h"

#include <tinysdl.h>

#include "ldtk.h"
#include "spritesheet.h"

#include <unordered_map>
#include <filesystem> 

using namespace TinySDL;
using namespace MicroNinja;

namespace fs = std::filesystem;

void Content::set_folder_name(const std::string & name) {
	folder_name = name;
}

// This implementation has to change afterwards since it depends
// on the location where the executable is being run.
fs::path Content::find_path() {
	auto exe_path = fs::current_path();
	auto new_content_path = fs::path(folder_name);

	for (int i = 0; i < 10; i++) {

		if (std::filesystem::is_directory(exe_path / new_content_path)) {
			return (exe_path / new_content_path);
		}

		new_content_path = ".." / new_content_path;
	}
	Log::warn("Content folder '%s' was not found.", folder_name.c_str());

	return "";
}

void Content::load_all() {

	content_path = find_path();

	Log::info("Content path: %s", content_path.generic_string().c_str());

	for (auto& item : fs::recursive_directory_iterator(content_path)) {
		if (item.is_regular_file()) {
			fs::path file_path = fs::relative(item.path(), content_path);
			std::string extension = file_path.extension().generic_string();
			file_path.replace_extension("");
			
			std::string key_name = file_path.generic_string();
			std::replace(key_name.begin(), key_name.end(), '\\', '/');

			if (extension == ".png") 
			{
				add_asset<Texture>(key_name, item.path().filename(), item.path().parent_path(), Texture::from_file(item.path().generic_string().c_str()));
			}

			if (extension == ".ldtk") 
			{
				add_asset<LDTk::File>(key_name, item.path().filename(), item.path().parent_path(), nlohmann::json::parse(File::load_txt(item.path().generic_string().c_str())));
			}			

		}
	}

	for (auto& item : fs::recursive_directory_iterator(content_path)) {
		if (item.is_regular_file()) {
			fs::path file_path = fs::relative(item.path(), content_path);
			std::string extension = file_path.extension().generic_string();
			file_path.replace_extension("");

			std::string key_name = file_path.generic_string();
			std::replace(key_name.begin(), key_name.end(), '\\', '/');

			if (extension == ".json")
			{
				add_asset<SpriteSheet>(key_name, item.path().filename(), item.path().parent_path(), { nlohmann::json::parse(File::load_txt(item.path().generic_string().c_str())), key_name });
			}
		}
	}
}
