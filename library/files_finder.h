#pragma once

#include <filesystem>

#include "copier_settings.h"


namespace fc
{
	class files_finder
	{
	public:
		files_finder(copier_settings settings, std::filesystem::path path);
		~files_finder() = default;

		files_finder(const files_finder&) = delete;
		files_finder(files_finder&&) noexcept = delete;

		files_finder& operator=(const files_finder&) = delete;
		files_finder& operator=(files_finder&&) noexcept = delete;

		const copier_settings& settings() const noexcept
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


	private:
		std::vector<std::filesystem::path> search_for_files() const;
		
		copier_settings settings_;
		std::filesystem::path path_;

		std::vector<std::filesystem::path> found_files_;
	};
}