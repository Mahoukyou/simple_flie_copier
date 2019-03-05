#pragma once

#include <vector>
#include <string>
#include <optional>

namespace fc
{
	struct copier_settings
	{
		std::optional<size_t> min_size{ std::nullopt };
		std::optional<size_t> max_size{ std::nullopt };

		std::vector<std::wstring> extensions_to_copy;
	};
}