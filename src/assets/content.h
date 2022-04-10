#pragma once 

#include <tinysdl.h>
#include <string>
#include <filesystem> 

#include <unordered_map>

namespace Lira {

	class Content {
		
		template <typename T>
		struct Asset {
			T data;
			std::filesystem::path file_name;
			std::filesystem::path folder;
		};

		std::string folder_name;
		std::filesystem::path find_path();

	public:
		void set_folder_name(const std::string& name);
		void load_all();
		
		template <typename T>
		static T* find(const std::string& name);

		template <typename T>
		static const std::unordered_map<std::string, Asset<T>>& find_all();
		
		template <typename T>
		static std::filesystem::path file_folder(const std::string& name);

		static std::filesystem::path get_content_path();

		static std::string normalize_key(const std::string& key);
	
	private:

		//The solution of using a template map is not is not ideal, 
		//since it's hard to clear the map afterwards. Perhaps it's better to
		//change it to a std::unordered_map<Type, std::unordered_map<std::string, Asset*>>.
		//But in that case all the asset classes should inherit from Asset.
		template <typename T>
		static inline std::unordered_map<std::string, Asset<T>> assets = {};

		static inline std::filesystem::path content_path = "";

		template <typename T>
		void add_asset(std::string key, std::filesystem::path file_name, std::filesystem::path folder, const T & asset);
	};

	inline std::filesystem::path Content::get_content_path() {
		return content_path;
	}

	inline std::string Content::normalize_key(const std::string& key) {
		std::filesystem::path new_key = get_content_path() / key;
		new_key = std::filesystem::absolute(new_key);
		return std::filesystem::relative(new_key, get_content_path()).generic_string();	
	}

	template <typename T>
	inline T* Content::find(const std::string& name) {
		T* data = nullptr;
		try
		{
			data = &(assets<T>.at(name).data);
		}
		catch (const std::out_of_range&)
		{
			Log::error("Content key \"%s\" not found", name.c_str());
		}
		return data;
	}

	template <typename T>
	inline const std::unordered_map<std::string, Content::Asset<T>>& Content::find_all() {
		return assets<T>;
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