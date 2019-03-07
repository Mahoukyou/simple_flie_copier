#include <iostream>
#include "files_finder.h"
#include "files_copier.h"

void file_copy_begin_callback(const fc::files_copier& fc, const size_t file_index)
{
	std::wcout << "Begin copy of: " << fc.finder().found_files()[file_index] << '\n';
}

void file_copy_end_callback(const fc::files_copier& fc, const std::error_code& error_code, const size_t file_index)
{
	if(!error_code)
	{
		std::wcout << fc.finder().found_files()[file_index] << " was copied successfully\n";
	}
	else
	{
		std::wcout << fc.finder().found_files()[file_index] << " was not copied\n";
	}
}

int main()
{
	fc::finder_settings finder_settings{};
	finder_settings.add_extension({ std::nullopt, std::nullopt, L"cpp" });
	finder_settings.add_extension({ std::nullopt, std::nullopt, L".h" });
	finder_settings.add_extension({ std::nullopt, std::nullopt, L".txt" });
	finder_settings.add_extension({ std::nullopt, std::nullopt, L"txt" });
	finder_settings.add_extension({ std::nullopt, std::nullopt, L".txt" });



	fc::copier_settings copier_settings{ L"C:\\Destination\\", fc::e_copy_options::keep_both };
	fc::files_copier fc{ copier_settings, finder_settings, L"A:\\tester" };

	fc.find_files_to_copy();
	for(auto const& file : fc.finder().found_files())
	{
		std::wcout << file.filename() << "\t\tSize: " << std::filesystem::file_size(file) << '\n';
	}

	std::wcout << std::flush;

	fc.copy_found_files(file_copy_begin_callback, file_copy_end_callback);

	std::wcout << std::flush;
}