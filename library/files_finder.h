#pragma once

#include <filesystem>
#include <optional>
#include <vector>

namespace fc
{
	struct extension_settings
	{
		extension_settings(std::optional<size_t> min_size, std::optional<size_t> max_size, std::wstring extension);

		const std::optional<size_t>& min_size() const noexcept
		{
			return min_size_;
		}

		const std::optional<size_t>& max_size() const noexcept
		{
			return max_size_;
		}

		const std::wstring& extension() const noexcept
		{
			return extension_;
		}

	private:
		const std::optional<size_t> min_size_;
		const std::optional<size_t> max_size_;

		std::wstring extension_;
	};

	inline bool operator==(const extension_settings& settings, const std::filesystem::path& extension)
	{
		return settings.extension() == extension;
	}

	struct finder_settings
	{
		explicit finder_settings(
			std::optional<size_t> global_min_size = {},
			std::optional<size_t> global_max_size = {},
			std::vector<extension_settings> extensions = {});

		bool add_extension(const extension_settings& extension);
		bool add_extension(extension_settings&& extension);

		// todo, emplace extension

		const std::optional<size_t>& global_min_size() const noexcept
		{
			return global_min_size_;
		}

		const std::optional<size_t>& global_max_size() const noexcept
		{
			return global_max_size_;
		}

		const std::vector<extension_settings>& extensions() const noexcept
		{
			return extensions_;
		}

	private:
		const std::optional<size_t> global_min_size_;
		const std::optional<size_t> global_max_size_;

		// Overrides global size settings per extension if not nullopt
		std::vector<extension_settings> extensions_;
	};

	class files_finder
	{
	public:
		enum class e_path_status
		{
			not_found,
			not_file_or_directory,
			ok
		};

		files_finder(finder_settings settings, std::filesystem::path path);
		~files_finder() = default;

		files_finder(const files_finder&) = delete;
		files_finder(files_finder&&) noexcept = delete;

		files_finder& operator=(const files_finder&) = delete;
		files_finder& operator=(files_finder&&) noexcept = delete;

		const finder_settings& settings() const noexcept
		{
			return settings_;
		}

		const std::filesystem::path& path() const noexcept
		{
			return path_;
		}

		const auto& found_files() const noexcept
		{
			return found_files_;
		}

		e_path_status path_status() const;

		bool update_files_in_path();

	private:
		std::vector<std::filesystem::path> search_for_files() const;

		finder_settings settings_;
		std::filesystem::path path_;

		std::vector<std::filesystem::path> found_files_;
	};
}