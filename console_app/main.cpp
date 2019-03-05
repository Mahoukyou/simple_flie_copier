#include <iostream>
#include "files_finder.h"

int main()
{
	fc::finder_settings settings{ 5000, std::nullopt, {} };
	settings.extensions_to_find.push_back({ 100, 1000, {L".cpp"} });
	settings.extensions_to_find.push_back({ std::nullopt, 10000, {L".h"} });

	fc::files_finder ff{ settings, R"(A:\test)" };
	ff.update_files_in_path();

	for (const auto& p : ff.found_files())
	{
		std::wcout << p.filename() << "\t\tSize: " << std::filesystem::file_size(p) << '\n';
	}
}
