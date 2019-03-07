#include "files_copier.h"

namespace fc
{
	files_copier::files_copier(copier_settings copier_settings, finder_settings finder_settings, std::filesystem::path finder_path) :
		settings_{ std::move(copier_settings) },
		files_finder_{ std::move(finder_settings), std::move(finder_path) }
	{

	}

	bool files_copier::find_files_to_copy()
	{
		return files_finder_.update_files_in_path();
	}

	bool files_copier::copy_found_files(
		const file_copy_begin_callback& begin_callback,
		const file_copy_end_callback& end_callback) const
	{
		if (finder().found_files().empty())
		{
			return true;
		}

		std::error_code error_code{};
		if (!exists(settings().destination, error_code) || !is_directory(settings().destination, error_code))
		{
			create_directories(settings().destination, error_code);

			if (error_code)
			{
				return false;
			}
		}

		const auto copy_option = static_cast<std::filesystem::copy_options>(settings().copy_option);
		for (size_t i = 0; i < finder().found_files().size(); ++i)
		{
			if (begin_callback)
			{
				begin_callback(*this, i);
			}

			std::filesystem::copy(finder().found_files()[i], settings().destination, copy_option, error_code);

			if (end_callback)
			{
				end_callback(*this, error_code, i);
			}
		}

		return true;
	}
}