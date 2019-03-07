#include "files_copier.h"
#include <cwchar>

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

		for (size_t i = 0; i < finder().found_files().size(); ++i)
		{
			if (begin_callback)
			{
				begin_callback(*this, i);
			}

			if(settings().copy_option == e_copy_options::keep_both)
			{
				auto new_filepath = get_new_filepath(finder().found_files()[i].filename());
				if (!exists(new_filepath, error_code))
				{
					new_filepath = get_next_free_filepath(new_filepath);
				}

				std::filesystem::copy(finder().found_files()[i], new_filepath, error_code);
			}
			else
			{
				const auto copy_option = static_cast<std::filesystem::copy_options>(settings().copy_option);
				std::filesystem::copy(finder().found_files()[i], settings().destination, copy_option, error_code);
			}

			if (end_callback)
			{
				end_callback(*this, error_code, i);
			}
		}

		return true;
	}

	std::filesystem::path files_copier::get_new_filepath(const std::wstring& filename) const
	{
		// todo, for now, make sure its valid in the settings
		auto filepath = settings().destination.wstring();
		if (wcsncmp(&filepath[filepath.size() - 1], L"\\", 1) != 0)
		{
			filepath += L'\\';
		}

		return filepath + filename;
	}

	std::filesystem::path files_copier::get_next_free_filepath(const std::filesystem::path& current_filepath)
	{
		const auto concatenate_new_filepath = [](const std::wstring& base_filepath, const size_t index, const std::wstring& extension) -> std::wstring
		{
			std::wstring new_filepath{};
			new_filepath.reserve(base_filepath.size() + 32);

			new_filepath += base_filepath;
			new_filepath += L" (";
			new_filepath += std::to_wstring(index);
			new_filepath += L")";
			new_filepath += extension;

			return new_filepath;
		};

		std::error_code error_code{};

		// good enough, the main bottleneck is querying the drive anyway
		const std::wstring extension = current_filepath.extension();
		std::wstring base_filepath = current_filepath;
		base_filepath.erase(base_filepath.size() - extension.size() - 1, extension.size());

		for(size_t i = 1; i != 0; ++i)
		{
			const auto new_filepath = concatenate_new_filepath(base_filepath, i, extension);
			if (!std::filesystem::exists(new_filepath, error_code))
			{
				return new_filepath;
			}
		}

		// todo, if we didn't find new filename, use A-Z on top of 1-...size_t
		return {};
	}
}
