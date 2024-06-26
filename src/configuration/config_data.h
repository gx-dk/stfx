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
	std::string structs_reader_class{"xml_reader_C"};	// within the generated structs module	
	std::string structs_writer_class{"xml_writer_C"};	// ... 
};

struct input_file
{
	std::string name;
};

struct input_spec
{
	std::vector<input_file> input;
};


struct uncommon_spec
	{
	std::vector<input_file> input;
	output_spec out;
	};


struct config
	{
	input_spec common_in_files;
	output_spec common_out_files;
	std::vector <uncommon_spec> non_common;
	};

