#include "files_finder.h"

namespace fc
{
	files_finder::files_finder(finder_settings settings, std::filesystem::path path) :
		settings_{ std::move(settings) },
		path_{ std::move(path) }
	{

	}

	files_finder::e_path_status files_finder::path_status() const
	{
		std::error_code dummy_error_code{};
		if (!exists(path(), dummy_error_code))
		{
			return e_path_status::not_found;
		}

		if (!(is_directory(path(), dummy_error_code) || is_regular_file(path(), dummy_error_code)))
		{
			return e_path_status::not_file_or_directory;
		}

		return e_path_status::ok;
	}

	// todo<?>, return the path status instead of boolean?
	bool files_finder::update_files_in_path()
	{
		if (path_status() != e_path_status::ok)
		{
			return false;
		}

		found_files_ = search_for_files();
		return true;
	}

	std::vector<std::filesystem::path> files_finder::search_for_files() const
	{
		using namespace std::filesystem;

		std::error_code error_code{};
		if (exists(path(), error_code) && is_regular_file(path(), error_code))
		{
			return { path() };
		}

		if (error_code)
		{
			return {};
		}

		std::vector<std::filesystem::path> found_files;
		const auto& extensions = settings().extensions_to_copy;
		for (auto& directory_entry : recursive_directory_iterator(path(), directory_options::skip_permission_denied, error_code))
		{
			if (is_regular_file(directory_entry.path(), error_code) &&
				std::find(extensions.begin(), extensions.end(), directory_entry.path().extension()) != extensions.end())
			{
				found_files.emplace_back(directory_entry.path());
			}
		}

		return found_files;
	}
}