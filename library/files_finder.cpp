#include "files_finder.h"

namespace fc
{
	extension_settings::extension_settings(std::optional<size_t> min_size, std::optional<size_t> max_size, std::wstring extension) :
		min_size_{ min_size },
		max_size_{ max_size },
		extension_{ std::move(extension) }
	{
		if (min_size_ && max_size_ &&
			min_size_.value() >= max_size_.value())
		{
			throw std::invalid_argument{ "Min size cannot be equal or greater than max size" };
		}

		// todo, other checks for extension? (/, |, \, <, > etc)?
		if (extension_.empty() || (extension_.size() == 1 && extension_ == L"."))
		{
			throw std::invalid_argument{ "Extension cannot be empty" };
		}

		if (wcsncmp(extension_.data(), L".", 1) != 0)
		{
			auto new_extension = L"." + extension_;
			extension_ = std::move(new_extension);
		}
	}

	finder_settings::finder_settings(
		std::optional<size_t> global_min_size,
		std::optional<size_t> global_max_size,
		std::vector<extension_settings> extensions) :
		global_min_size_{ global_min_size },
		global_max_size_{ global_max_size },
		extensions_{ std::move(extensions) }
	{
		if (global_min_size_ && global_max_size_ &&
			global_min_size_.value() >= global_max_size_.value())
		{
			throw std::invalid_argument{ "Min size cannot be equal or greater than max size" };
		}

		// todo, extensions_ unique items!!
	}

	bool finder_settings::add_extension(const extension_settings& extension)
	{
		const auto find_extension_predicate = [&extension](const extension_settings& ext) -> bool
		{
			return ext.extension() == extension.extension();
		};

		if (std::find_if(extensions().begin(), extensions().end(), find_extension_predicate) == extensions().end())
		{
			return false;
		}

		extensions_.push_back(extension);
		return true;
	}

	bool finder_settings::add_extension(extension_settings&& extension)
	{
		const auto find_extension_predicate = [&extension](const extension_settings& ext) -> bool
		{
			return ext.extension() == extension.extension();
		};

		if (std::find_if(extensions().begin(), extensions().end(), find_extension_predicate) != extensions().end())
		{
			return false;
		}

		extensions_.push_back(std::move(extension));
		return true;
	}

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

		if (error_code || settings().extensions().empty())
		{
			return {};
		}

		std::vector<std::filesystem::path> found_files;
		const auto& extensions = settings().extensions();
		for (auto& directory_entry : recursive_directory_iterator(path(), directory_options::skip_permission_denied, error_code))
		{
			if (!directory_entry.is_regular_file(error_code))
			{
				continue;
			}

			if (const auto found_it = std::find(extensions.begin(), extensions.end(), directory_entry.path().extension());
				found_it != extensions.end())
			{
				const auto file_size = directory_entry.file_size(error_code);
				if (error_code)
				{
					// todo, do something with the file, since it matches the extension, but file_size could not be read
					continue;
				}

				// todo, use global only when both locals are null<!> because otherwise, it might have some undesired effects
				// eg. local min 500, global max 200 
				const auto& min_size_settings = found_it->min_size() ? found_it->min_size() : settings().global_min_size();
				const auto& max_size_settings = found_it->max_size() ? found_it->max_size() : settings().global_max_size();

				if (min_size_settings && min_size_settings.value() > file_size)
				{
					continue;
				}

				if (max_size_settings && max_size_settings.value() < file_size)
				{
					continue;
				}

				found_files.emplace_back(directory_entry.path());
			}
		}

		return found_files;
	}
}