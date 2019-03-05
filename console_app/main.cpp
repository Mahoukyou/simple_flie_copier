#include <iostream>
#include "files_finder.h"

int main()
{
	fc::finder_settings settings{ 100, 500, {} };
	settings.extensions_to_copy.emplace_back(L".cpp");
	settings.extensions_to_copy.emplace_back(L".h");

	fc::files_finder ff{ settings, R"(A:\)" };
	ff.update_files_in_path();

	for (const auto& p : ff.found_files())
	{
		std::wcout << p.filename() << "\t\tSize: " << std::filesystem::file_size(p) << '\n';
	}
}
