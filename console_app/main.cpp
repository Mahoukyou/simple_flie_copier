#include "files_finder.h"
#include <iostream>

int main()
{
	fc::copier_settings settings;
	settings.extensions_to_copy.emplace_back(L".txt");

	fc::files_finder ff{ settings, R"(A:\)" };
	ff.update_files_in_path();

	for(const auto& p : ff.found_files())
	{
		std::wcout << p << '\n';
	}
}