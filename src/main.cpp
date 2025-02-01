// main.cpp

#include <filesystem>
#include <string>

#include <fmt/format.h>

#include "items.h"			// parser.hpp *SHOULD* have included this... but didn't so...

#include "config_data.h"
#include "enums.h"
#include "process_items_ini.h"
#include "process_items_xml.h"
#include "structs.h"
#include "timestamp.h"

int process_single_header(std::filesystem::path in_file);
int process_stfx_file(std::filesystem::path in_file);

int main(int argc, char *argv[])
	{
	int rv{};

	fmt::println("{}", app_timestamp);

	if (argc <= 1)
		{
		fmt::println("{} <filename> :\n"
			"\nProgram that creates code that performs the translation of Structs To/From Xml file\n\n"
			"<filename> is <inputheader> headerfile or <setup>.stfx stfx configuration file.\n"
			"<filename> parameter must be given",
			argv[0]);
		exit(1);
		}
	std::filesystem::path in_path(argv[1]);
	if (std::filesystem::exists(in_path) == false)
		{
		fmt::println("{} <filename> :\n"
			"\nProgram that creates code that performs the translation of Structs To/From Xml file\n\n"
			"<filename> is <inputheader> headerfile or <setup>.stfx stfx configuration file.\n"
			"<filename> parameter must be given\n"
			"<filename> '{}' does NOT exist",
			argv[0], in_path.string());
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
	if (rv == 0)
		{
		fmt::println("\nProcessing successful. Returning {} to caller", rv);
		}
	else
		{
		fmt::println("\nProcessing failed. Returning {} to caller", rv);
		}
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
		fmt::println("Successfully parsed file:\t{}", in_path.string());
		std::filesystem::path base_dir_path = in_path.parent_path();
		std::vector<std::string> input;
		output_spec output;

		input.push_back(in_path.filename().string());		// filename only ... path to file is in base_dir_path.. 
		output.enum_file = "enums";
		output.structs_file = "structs";
		output.structs_reader_class = "xml_reader_C";
		output.structs_writer_class = "xml_writer_C";
		output.file_type = serialization_type_E::xml_attrib;
		process_items_xml_attrib_C process_items(base_dir_path);
		ok = process_items.process_items(info_items, input, output);
		fmt::println("Processing items done. Success = {}", ok);
		}
	else
		{
		fmt::println("Failed to correctly parse '{}' code {}", in_path.string(), rv);
		}

	rv = (ok == true) ? 0 : 1;		// yes... 0 = good !! 
	return rv;
	}

int process_stfx_file(std::filesystem::path in_file)
	{
	int rv{};
	xml_reader_C reader;
	config conf;
	bool ok;
	std::vector<std::string> common_input_files;

	ok = reader.read_from_file(in_file.string(), conf);
	if (ok == false)
		{
		fmt::println("failed to process {}", in_file.string());
		rv = 1;		// fail !! 
		return rv;	// and exit
		}

	std::filesystem::path base_dir_path = in_file.parent_path();

	// process common input files
	info_items_C common_items;
	// read input files
	for (const auto &filename : conf.common_in_files.input)
		{
		bool ok;
		std::filesystem::path path(base_dir_path);
		path /= filename.name;
		ok = common_items.process_input_file(path);
		if (ok == true)
			{
			fmt::println("Successfully parsed file:\t{}", path.string());
			}
		else
			{
			fmt::println("ERROR : failed to process {}", path.string());
			}
		common_input_files.push_back(filename.name);
		}

	fmt::println("");

	output_spec common_output_files = conf.common_out_files;
	switch (common_output_files.file_type)
		{
		case serialization_type_E::xml:
			if (true)
				{
				process_items_xml_C process_items(base_dir_path);
				process_items.process_items(common_items, common_input_files, common_output_files);
				}
			break;
		case serialization_type_E::xml_attrib:
			if (true)
				{
				process_items_xml_attrib_C process_items(base_dir_path);
				process_items.process_items(common_items, common_input_files, common_output_files);
				}
			break;
		case serialization_type_E::ini:
			if (true)
				{
				process_items_ini_C process_items(base_dir_path);
				process_items.process_items(common_items, common_input_files, common_output_files);
				}
			break;
		case serialization_type_E::n_ini:
			if (true)
				{
				process_items_n_ini_C process_items(base_dir_path);
				process_items.process_items(common_items, common_input_files, common_output_files);
				}
			break;
		case serialization_type_E::json:
		default:
			fmt::println("ERROR : output type '{}' not currently supported", common_output_files.file_type);
			break;
		}

	// now process the "uncommon" files ... which are common plus local part 
	for (const auto &uncom : conf.non_common)
		{
		info_items_C plus_items(common_items);
		std::vector<std::string> plus_input_files(common_input_files);
		for (const auto &filename : uncom.input)
			{
			bool ok;
			std::filesystem::path path(base_dir_path);
			path /= filename.name;
			ok = plus_items.process_input_file(path);
			if (ok == true)
				{
				fmt::println("Successfully parsed file:\t{}", path.string());
				}
			else
				{
				fmt::println("ERROR : failed to process {}", path.string());
				}
			plus_input_files.push_back(filename.name);
			}
		output_spec plus_output_files = uncom.out;
		switch (plus_output_files.file_type)
			{
			case serialization_type_E::xml:
				if (true)
					{
					process_items_xml_C process_items(base_dir_path);
					process_items.process_items(plus_items, plus_input_files, plus_output_files);
					}
				break;
			case serialization_type_E::xml_attrib:
				if (true)
					{
					process_items_xml_attrib_C process_items(base_dir_path);
					process_items.process_items(plus_items, plus_input_files, plus_output_files);
					}
				break;
			case serialization_type_E::ini:
				if (true)
					{
					process_items_ini_C process_items(base_dir_path);
					process_items.process_items(plus_items, plus_input_files, plus_output_files);
					}
				break;
			case serialization_type_E::n_ini:
				if (true)
					{
					process_items_n_ini_C process_items(base_dir_path);
					process_items.process_items(plus_items, plus_input_files, plus_output_files);
					}
				break;
			case serialization_type_E::json:
			default:
				fmt::println("ERROR : output type '{}' not currently supported", plus_output_files.file_type);
				break;
			}
		}

	rv = (ok == true) ? 0 : 1;		// yes... 0 = good !! 

	return rv;
	}
