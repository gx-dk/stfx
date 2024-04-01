// main.cpp

#include <filesystem>
#include <string>
#include <fmt/format.h>

#include "items.h"			// parser.hpp *SHOULD* have included this... but didn't so...


#include "process_items.h"
#include "config_data.h"

#include "structs.h"

int process_single_header(std::filesystem::path in_file);
int process_stfx_file(std::filesystem::path in_file);

int main(int argc, char *argv[])
	{
	int rv{};
	if (argc <= 1)
		{
		fmt::print("{} <filename> :\n"
			"\nProgram that creates code that performs the translation of Structs To/From Xml file\n\n"
			"<filename> is <inputheader> headerfile or <setup>.stfx stfx configuration file.\n"
			"<filename> parameter must be given\n", argv[0]);
		exit(1);
		}
	std::filesystem::path in_path(argv[1]);
	if (std::filesystem::exists(in_path) == false)
		{
		fmt::print("{} <filename> :\n"
			"\nProgram that creates code that performs the translation of Structs To/From Xml file\n\n"
			"<filename> is <inputheader> headerfile or <setup>.stfx stfx configuration file.\n"
			"<filename> parameter must be given\n\n"
			"<filename> '{}' does NOT exist\n", argv[0], in_path.string());
		exit(1);
		}
	in_path = std::filesystem::canonical(in_path);
	std::filesystem::path ext = in_path.extension();
	if (ext == ".stfx")
		{
		rv = process_stfx_file(in_path);
		}
	else
		{
		rv = process_single_header(in_path);
		}
	fmt::println("Returning {} to caller", rv);
	return rv;
	}

int process_single_header(std::filesystem::path in_path)
	{
	int rv{};

	info_items_C info_items;
	bool ok = info_items.process_input_file(in_path);
	rv = info_items.get_parse_rv();

	if (ok == true)
		{
		bool ok;
		fmt::print("Successfully parsed file: {}\n", in_path.string());
		in_out_spec file_specs;
		file_specs.in_file = in_path.string();
		file_specs.out.enum_file = "enums";
		file_specs.out.structs_file = "structs";
		process_items_C process_items;
		ok = process_items.process_items(info_items, file_specs);
		std::filesystem::path base_dir_path = in_path.parent_path();
		process_items_C process_items(base_dir_path);
		fmt::print("Processing items done. Success = {}", ok);
		}
	else
		{
		fmt::print("Failed to correctly parse '{}' code {}", in_path.string(), rv);
		}
	return rv;
	}

int process_stfx_file(std::filesystem::path in_file)
	{
	int rv{};
	xml_reader reader;
	config conf;
	bool ok;

	ok = reader.read_from_file(in_file.string(), conf);
	if (ok == false)
		{
		fmt::println("failed to process {}", in_file.string());
		rv = 1;		// fail !! 
		return rv;	// and exit
		}
	// temp... write out what we have ... 
	xml_writer writer(false);
	writer.write_to_file("test.stfx", conf);

	std::filesystem::path base_dir_path = in_file.parent_path();
	process_items_C process_items(base_dir_path);
	rv = (ok == true) ? 0 : 1;		// yes... 0 = good !! 

	return rv;
	}
