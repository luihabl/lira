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
		
		template <typename T>
		static std::filesystem::path file_folder(const std::string& name);

		static std::filesystem::path get_content_path();
	
	private:
		template <typename T>
		struct Asset {
			T data;
			std::filesystem::path file_name;
			std::filesystem::path folder;
		};

		template <typename T>
		static inline std::unordered_map<std::string, Asset<T>> assets = {};

		static inline std::filesystem::path content_path = "";

		template <typename T>
		void add_asset(std::string key, std::filesystem::path file_name, std::filesystem::path folder, const T & asset);
	};

	inline std::filesystem::path Content::get_content_path() {
		return content_path;
	}

	template <typename T>
	inline T* Content::find(const std::string& name) {
		return &(assets<T>[name].data);
	}

	template <typename T>
	inline std::filesystem::path Content::file_folder(const std::string& name) {
		return assets<T>[name].folder;
	}

	template <typename T>
	inline void Content::add_asset(std::string key, std::filesystem::path file_name, std::filesystem::path folder, const T & asset) {
		assets<T>[key] = {
			asset,
			file_name,
			folder
		};
	}
}