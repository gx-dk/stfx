// config_data.h
// items for stfx that can be configured (via xml file)
//

#pragma once

#include <string>
#include <vector>


struct output_spec
	{
	std::string relative_directory {};		// path relative to input file
	std::string enum_file {};				// file root name of enums files (.cpp and .h)
	std::string structs_file {};			// file root name of enums files (.cpp and .h)
	};

struct common_input
	{
	std::string in_file{};
	};

struct in_out_spec
	{
	std::string in_file{};
	output_spec out;
	};


struct config
	{
	std::vector <common_input> common_in_files;
	output_spec common_out_files;
	std::vector <in_out_spec> in_out_files;
	};

