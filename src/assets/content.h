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
	
	};
}