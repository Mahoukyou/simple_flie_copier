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
				// todo, for now, make sure its valid in the settings
				auto dest_wstring = settings().destination.wstring();
				if (wcsncmp(&dest_wstring[dest_wstring.size() - 1], L"\\", 1) != 0)
				{
					dest_wstring += L'\\';
				}

				dest_wstring += finder().found_files()[i].filename();
				
				// todo, check if the file exists before calling the method
				// so we won't have to do redundant copy of the current filename
				const auto new_file_name = get_next_free_filename(dest_wstring);

				std::filesystem::copy(finder().found_files()[i], new_file_name, error_code);

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

	std::filesystem::path files_copier::get_next_free_filename(const std::filesystem::path& current_filename)
	{
		if(!exists(current_filename))
		{
			return current_filename;
		}

		// hacky and not efficient, redo entire method later, poc for now
		const std::wstring extension = current_filename.extension();
		std::wstring base_filename = current_filename;
		base_filename.erase(base_filename.size() - extension.size() - 1, extension.size());

		for(size_t i = 1; i != 0; ++i)
		{
			auto new_filename = base_filename;
			new_filename += L" (";
			new_filename += std::to_wstring(i);
			new_filename += L")";
			new_filename += extension;

			if (!std::filesystem::exists(new_filename))
			{
				return new_filename;
			}
		}

		// todo, if we didn't find new filename, use A-Z on top of 1-...size_t
		return {};
	}
}
