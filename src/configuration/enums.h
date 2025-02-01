// enums.h
//
// Automatically generated using stfx
// Do not directly edit this file, use stfx to re-create this file.
//
// License : MIT License
//
// stfx built at : Feb  1 2025 05:29:12, Software Version : 25.01

#pragma once

#include <string>
#include <fmt/format.h>

#include "config_data.h"

namespace stfx
	{
	const std::string s_from_e(serialization_type_E e);
	bool s_to_e(std::string s, serialization_type_E &e);
	};

template <> struct fmt::formatter < serialization_type_E > : formatter<string_view>
	{
	template <typename FormatContext>
	auto format(serialization_type_E c, FormatContext& ctx) const
		{
		std::string str = stfx::s_from_e(c);
		string_view name = str;
		return formatter<string_view>::format(name, ctx);
		}
	};
