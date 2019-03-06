#pragma once

#include <vector>
#include <filesystem>
#include <functional>
#include "files_finder.h"

namespace fc
{
	enum class e_copy_options
	{
		skip_existing = static_cast<int>(std::filesystem::copy_options::skip_existing),
		overwrite_existing = static_cast<int>(std::filesystem::copy_options::overwrite_existing),
		
		// Updates an existing file in destination if last modification of source file is newer
		update_existing = static_cast<int>(std::filesystem::copy_options::update_existing)
	};

	struct copier_settings
	{
		std::filesystem::path destination;
		e_copy_options copy_option{ e_copy_options::skip_existing };
	};

	class files_copier
	{
	public:
		/* */
		using file_copy_begin_callback = std::function<void(const files_copier&, size_t)>;
		using file_copy_end_callback = std::function<void(const files_copier&, bool, size_t)>;

		files_copier(copier_settings copier_settings, finder_settings finder_settings, std::filesystem::path finder_path);
		~files_copier() = default;

		files_copier(const files_copier&) = delete;
		files_copier(files_copier&&) noexcept = delete;

		files_copier& operator=(const files_copier&) = delete;
		files_copier& operator=(files_copier&&) noexcept = delete;

		const copier_settings& settings() const noexcept
		{
			return settings_;
		}

		const files_finder& finder() const noexcept
		{
			return files_finder_;
		}

		bool find_files_to_copy();
		void copy_found_files(
			const file_copy_begin_callback& begin_callback,
			const file_copy_end_callback& end_callback) const;

	private:
		copier_settings settings_;
		files_finder files_finder_;
	};
}
