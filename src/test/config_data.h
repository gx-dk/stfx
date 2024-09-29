// config_data.h
// items for stfx that can be configured (via xml file)
//

#pragma once

#include <string>
#include <vector>

enum class may_ish_E
{
	yes,
	maybe,
	no
};
	

struct test_data
	{
	unsigned int ui{ 42 };
	int i{ -42 };
	unsigned short us{ 42 };
	short s{ -42 };
	unsigned long ul{ 42 };
	long l{ -42 };
	float f{ 42.01f };
	double d{ 42.0001 };
	std::string str{ "testing 1 2 3 4 5" };
	may_ish_E maybe{};
	};

struct output_spec
	{
	std::string relative_directory {};		// path relative to input file
	std::string enum_file {};				// file root name of enums files (.cpp and .h)
	std::string structs_file {};			// file root name of enums files (.cpp and .h)
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
	test_data erik;
	};

