// process_items.cpp

#include "process_items.h"

#include <chrono>
#include <cstdio>
#include <filesystem>
#include <string>

#include <fmt/format.h>
#include <fmt/chrono.h>

#include "items.h"
#include "config_data.h"
#include "timestamp.h"

process_items_C::process_items_C(std::filesystem::path base_dir_path) : m_base_dir_path(base_dir_path)
	{
	m_stfx_info += fmt::format(	"//\n"
								"// Automatically generated using stfx\n"
								"// Do not directly edit this file, use stfx to re-create this file.\n"
								"//\n"
								"// License : MIT License\n"
								"//\n"
								"// {}\n", app_timestamp);
	}

bool process_items_C::process_items(info_items_C &items, const std::vector<std::string> &input_files, const output_spec &output)
	{
	bool rv{ false };

	fmt::println("\nStarting to create output.");

	rv = fixup_types_of_names(items);
	rv &= find_top_struct(items);
	if (output.enum_file.empty() || output.enum_file == "-")
		{
		fmt::println("Not generating enum information at this point");
		}
	else
		{
		rv &= process_all_enums(items, input_files, output);
		}
	if (output.structs_file.empty() || output.structs_file == "-")
		{
		fmt::println("Not generating struct information at this point");
		}
	else
		{
		rv &= process_all_structs(items, input_files, output);
		}
	return rv;
	}


bool process_items_C::process_all_enums(info_items_C &items, const std::vector<std::string> &input_files, const output_spec &output)
	{
	bool rv{ true };
	std::filesystem::path outpath{ m_base_dir_path };
	if (output.relative_directory.empty() == false)
		{
		outpath = outpath /= output.relative_directory;
		std::filesystem::create_directories(outpath);
		outpath = std::filesystem::canonical(outpath);
		}
	std::filesystem::path out_filepath_cpp = outpath /= output.enum_file + ".cpp";
	std::filesystem::path out_filepath_h = outpath.replace_extension(".h");
	std::string out_pathfilename_cpp{ out_filepath_cpp.string() };
	std::string out_pathfilename_h{ out_filepath_h.string() };
	std::string out_filename_cpp{ out_filepath_cpp.filename().string() };
	std::string out_filename_h{ out_filepath_h.filename().string() };

	auto &enums = items.get_enums();

	std::FILE *f_cpp;
	f_cpp = std::fopen(out_pathfilename_cpp.c_str(), "w");
	if (f_cpp != nullptr)
		{
		fmt::println("Opened enum .cpp file :\t\t{}", out_pathfilename_cpp);
		std::FILE *f_h;
		f_h = std::fopen(out_pathfilename_h.c_str(), "w");
		if (f_h != nullptr)
			{
			fmt::println("Opened enum .h file :\t\t{}", out_pathfilename_h);
			rv = true;		// files are open

			fmt::println(f_cpp, "// {}", out_filename_cpp);
			fmt::println(f_cpp, "{}", m_stfx_info);
			fmt::println(f_cpp, "#include \"{}\"\n", out_filename_h);
			fmt::println(f_cpp, "#include <string>\n#include <map>\n");
			for (const auto &in_filename : input_files)
				{
				fmt::println(f_cpp, "#include \"{}\"", in_filename);
				}
			fmt::println(f_cpp, "");

			fmt::println(f_h, "// {}", out_filename_h);
			fmt::println(f_h, "{}", m_stfx_info);
			fmt::println(f_h, "#pragma once\n");
			fmt::println(f_h, "#include <string>");
			fmt::println(f_h, "#include <fmt/format.h>\n");
			for (const auto &in_filename : input_files)
				{
				fmt::println(f_h, "#include \"{}\"", in_filename);
				}
			fmt::println(f_h, "");
			fmt::println(f_h, "namespace stfx\n\t{{");	// start namespace ! 

			for (auto &pair : enums)
				{
				const enum_S &e = pair.second;
				process_enum(e, f_cpp, f_h);
				}

			fmt::println(f_h, "\t}};");		// finish namespace

			// now generate fmt::formatter items
			for (auto &pair : enums)
				{
				const enum_S &e = pair.second;
				create_fmt_templates_for_enum(e, f_h);
				}


			std::fclose(f_h);
			}
		else
			{
			fmt::println("\nERROR : Failed to open output file {}", out_filename_h);
			}
		std::fclose(f_cpp);
		}
	else
		{
		fmt::println("\nERROR : Failed to open output file {}", out_filename_cpp);
		}
	return rv;
	}

bool process_items_C::process_enum(const enum_S &the_enum, std::FILE *out_file_cpp, std::FILE *out_file_h)
	{
	// s_from_e

	fmt::println(out_file_h, "\tconst std::string s_from_e({0} e);", the_enum.name);

	fmt::println(out_file_cpp, "const std::string stfx::s_from_e({0} e)\n\t{{", the_enum.name);

	fmt::println(out_file_cpp, "\tstd::map < {0}, std::string > {0}_to_string =\n\t\t{{", the_enum.name);

	for (auto &ent : the_enum.enums)
		{
		if (the_enum.is_class_enum == true)
			{
			fmt::println(out_file_cpp, "\t\t\t{{ {0}::{1}, \"{1}\" }},", the_enum.name, ent.name);
			}
		else
			{
			fmt::println(out_file_cpp, "\t\t\t{{ {0}, \"{0}\" }},", ent.name);
			}
		}
	fmt::println(out_file_cpp, "\t\t}};");
	fmt::println(out_file_cpp, "\tstd::string rv = ({0}_to_string.find(e) != {0}_to_string.end()) ? {0}_to_string[e] : std::to_string(int(e));", the_enum.name);
	fmt::println(out_file_cpp, "\treturn rv;\n\t}};\n");

	// s_to_e

	fmt::println(out_file_h, "\tbool s_to_e(std::string s, {} &e);", the_enum.name);

	fmt::println(out_file_cpp, "bool stfx::s_to_e(std::string s, {} &e)\n\t{{", the_enum.name);
	fmt::println(out_file_cpp, "\tbool rv {{false}};\n");

	fmt::println(out_file_cpp, "\tstd::map < std::string, {0} > string_to_{0} =\n\t\t{{", the_enum.name);

	for (auto &ent : the_enum.enums)
		{
		if (the_enum.is_class_enum == true)
			{
			fmt::println(out_file_cpp, "\t\t\t{{ \"{1}\", {0}::{1} }},", the_enum.name, ent.name);
			}
		else
			{
			fmt::println(out_file_cpp, "\t\t\t{{ \"{0}\", {0} }},", ent.name);
			}
		}
	fmt::println(out_file_cpp, "\t\t}};");
	fmt::println(out_file_cpp,
		"\tif(string_to_{0}.find(s) != string_to_{0}.end())\n"
		"\t\t{{\n"
		"\t\te = string_to_{0}[s];\n"
		"\t\trv = true;\n"
		"\t\t}}\n"
		"\telse\n"
		"\t\t{{\n"
		"\t\ttry\n"
		"\t\t\t{{\n"
		"\t\t\tint i = std::stoi(s);\t\t// just in case the value of enum has been given\n"
		"\t\t\te = {0}(i);\n"
		"\t\t\t}}\n"
		"\t\tcatch (std::exception &)\n"
		"\t\t\t{{\n"
		"\t\t\t// unexpected string, will just be ignored (for now!)... This *COULD* throw an exception .. \n"
		"\t\t\t}}\n"
		"\t\t// note rv NOT set to true ! \n"
		"\t\t}}", the_enum.name);

	fmt::println(out_file_cpp, "\treturn rv;\n\t}};\n");
	return true;
	}

bool process_items_C::create_fmt_templates_for_enum(const enum_S &the_enum, std::FILE *out_file_h)
	{
	fmt::println(out_file_h,
		"\n"
		"template <> struct fmt::formatter < {0} > : formatter<string_view>\n"
		"\t{{\n"
		// parse is inherited from formatter<string_view>.
		"\ttemplate <typename FormatContext>\n"
		"\tauto format({0} c, FormatContext& ctx) const\n"
		"\t\t{{\n"
		"\t\tstd::string str = stfx::s_from_e(c);\n"
		"\t\tstring_view name = str;\n"
		"\t\treturn formatter<string_view>::format(name, ctx);\n"
		"\t\t}}\n"
		"\t}};", the_enum.name);
	return true;
	}


// look though all items in the struct dictionary ... and fixup enum_or_struct to the correct type!!
bool process_items_C::fixup_types_of_names(info_items_C &items)
	{
	bool rv{ true };
	auto &structs = items.get_structs();
	for (auto &pair : structs)
		{
		struct_S &s = pair.second;
		for (struct_line_complex_S &co : s.complex)
			{
			if (complex_item_type_E line_type = co.line_type; line_type == complex_item_type_E::enum_or_struct_E)
				{
				co.line_type = items.get_item_type(co.type_name);
				if (co.line_type == complex_item_type_E::unknown_E)
					rv = false;
				}
			}
		}
	return rv;
	}

bool process_items_C::find_top_struct(info_items_C &items)
	{
	bool rv{ true };
	auto &structs = items.get_structs();
	for (auto &pair : structs)
		{
		struct_S &s = pair.second;
		for (struct_line_complex_S &co : s.complex)
			{
			switch (co.line_type)
				{
				case complex_item_type_E::vector_E:
				case complex_item_type_E::struct_E:
					if (auto &child = structs.find(co.type_name); child != structs.end())
						{
						child->second.incoming_count++;
						}
					break;
				default:
					break;
				}
			}
		}
	return rv;
	}