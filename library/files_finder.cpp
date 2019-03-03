#include "files_finder.h"

namespace fc
{
	files_finder::files_finder(copier_settings settings, std::filesystem::path path) :
		settings_{ std::move(settings) },
		path_{ std::move(path) }
	{

	}

	std::vector<std::filesystem::path> files_finder::search_for_files() const
	{
		std::error_code error_code{};

		return { };
	}
}
