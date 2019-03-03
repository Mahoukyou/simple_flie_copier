#include "files_finder.h"

namespace fc
{
	files_finder::files_finder(copier_settings settings, std::filesystem::path path) :
		settings_{ std::move(settings) },
		path_{ std::move(path) }
	{

	}

	void files_finder::update_files_in_path()
	{
		found_files_ = search_for_files();
	}

	std::vector<std::filesystem::path> files_finder::search_for_files() const
	{
		using namespace std::filesystem;

		std::error_code error_code{};

		if(exists(path(), error_code) && is_regular_file(path(), error_code))
		{
			return { path() };
		}

		// todo, error

		std::vector<std::filesystem::path> found_files;
		const auto& extensions = settings().extensions_to_copy;
		for(auto& directory_entry : recursive_directory_iterator(path(), directory_options::skip_permission_denied, error_code))
		{
			if(is_regular_file(directory_entry.path(), error_code) && 
				std::find(extensions.begin(), extensions.end(), directory_entry.path().extension()) != extensions.end())
			{
				found_files.emplace_back(directory_entry.path());
				continue;
			}

			// todo, error code
		}

		// todo, any left error

		return found_files;
	}
}
