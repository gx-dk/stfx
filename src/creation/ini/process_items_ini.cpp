//process_items_ini.cpp

#include "process_items_ini.h"

#include <chrono>
#include <cstdio>
#include <filesystem>
#include <string>

#include <fmt/format.h>
#include <fmt/chrono.h>

#include "items.h"
#include "config_data.h"

bool process_items_ini_C::process_all_structs(info_items_C &items, const std::vector<std::string> &input_files, const output_spec &output)
	{
	bool rv{ false };
	std::filesystem::path outpath{ m_base_dir_path };
	if (output.relative_directory.empty() == false)
		{
		outpath = outpath /= output.relative_directory;
		std::filesystem::create_directories(outpath);
		outpath = std::filesystem::canonical(outpath);
		}
	std::filesystem::path out_filepath_cpp = outpath /= output.structs_file + ".cpp";
	std::filesystem::path out_filepath_h = outpath.replace_extension(".h");
	std::string out_pathfilename_cpp{ out_filepath_cpp.string() };
	std::string out_pathfilename_h{ out_filepath_h.string() };
	std::string out_filename_cpp{ out_filepath_cpp.filename().string() };
	std::string out_filename_h{ out_filepath_h.filename().string() };
	std::string enums_filename_h{ output.enum_file + ".h" };
	std::string reader_class_name = output.structs_reader_class;
	std::string writer_class_name = output.structs_writer_class;
	auto &structs = items.get_structs();

	std::FILE *f_cpp;
	f_cpp = std::fopen(out_pathfilename_cpp.c_str(), "w");
	if (f_cpp != nullptr)
		{
		fmt::println("Opened ini struct .cpp file :\t{}", out_pathfilename_cpp);
		std::FILE *f_h;
		f_h = std::fopen(out_pathfilename_h.c_str(), "w");
		if (f_h != nullptr)
			{
			fmt::println("Opened ini struct .h file :\t{}", out_pathfilename_h);
			rv = true;

			fmt::println(f_cpp, "// {0}\n// created {1}", out_filename_cpp, m_timestamp);
			fmt::println(f_cpp, "{}", made_by);
			fmt::println(f_cpp, "#include \"{}\"\n", out_filename_h);
			fmt::println(f_cpp, "#include <string>\n"
				"#include <map>\n"
				"#include <regex>\n"
				"#include <stdexcept>");
			if (output.enum_file.empty() || output.enum_file == "-")
				{
				fmt::println(f_cpp, "// not including blank enum file");
				}
			else
				{
				fmt::println(f_cpp, "#include \"{}\"", enums_filename_h);
				}
			for (const auto &in_filename : input_files)
				{
				fmt::println(f_cpp, "#include \"{}\"", in_filename);
				}

			fmt::println(f_h, "// {0}\n// created {1}", out_filename_h, m_timestamp);
			fmt::println(f_h, "{}", made_by);

			fmt::println(f_h,
				"#pragma once\n"
				"\n"
				"#include <string>\n"
				"#include <map>\n"
			);
			for (const auto &in_filename : input_files)
				{
				fmt::println(f_h, "#include \"{}\"", in_filename);
				}
			fmt::println(f_h, 
				"\n"
				"struct read_item_S\n"
				"\t{{\n"
				"\tstd::map<std::string, std::string>items;\n"
				"\tstd::map<std::string, read_item_S>under;\n"
				"\t}};\n"
				"\n"
				);


			// reader class ===============================================================
			fmt::println(f_h,
				"class {0}\n"
				"\t{{\n"
				"\tprivate:\n"
				"\t\tread_item_S m_lookup;\n"
				"\tpublic:\n"
				"\t\t{0}();", reader_class_name);

			fmt::println(f_cpp,
				"\n"
				"{0}::{0}()\n"
				"\t{{\n"
				"\t}}\n", reader_class_name);

			for (auto &pair : structs)
				{
				const struct_S &s = pair.second;
				if (s.incoming_count == 0)
					{
					fmt::println(f_h, "\t\tvirtual bool read_from_file(std::string const &filename, {} &struct_to_fill);", s.name);

					fmt::println(f_cpp,
						"bool {0}::read_from_file(std::string const &filename, {1} &struct_to_fill)\n"
						"\t{{\n"
						"\tbool rv{{}};\n"
						"\tstd::FILE *file{{}};\n"
						"\tfile = std::fopen(filename.c_str(), \"r\");\n"
						"\tif (file != nullptr)\n"
						"\t\t{{\n"
						"\t\tread_item_S *current_lookup = &m_lookup;\n"
						"\t\tconst int buflen = 80;\n"
						"\t\tchar buf[buflen + 1];\n"
						"\t\twhile(nullptr != std::fgets(buf, buflen, file))\n"
						"\t\t\t{{\n"
						"\t\t\tstd::string line{{ buf }};\n"
						"\t\t\tsize_t pos = line.find('%');\t// remove remainder of line after comment character... \n"
						"\t\t\tif (pos != std::string::npos)\n"
						"\t\t\t\t{{\n"
						"\t\t\t\tline = line.substr(0, pos);\n"
						"\t\t\t\t}}\n"
						"\t\t\tif (line.length() == 0)\n"
						"\t\t\t\tcontinue;\n"
						"\n"
						"\t\t\tstd::smatch res;\n"
						"\t\t\tstd::regex regex;\n"
						"\t\t\tswitch (line[0])\n"
						"\t\t\t\t{{\n"
						"\t\t\t\tcase '[':\n"
						"\t\t\t\t\tregex = \"\\\\[(\\\\w+)\\\\]\";\n"
						"\t\t\t\t\tif (std::regex_search(line, res, regex) == true)\n"
						"\t\t\t\t\t\t{{\n"
						"\t\t\t\t\t\tstd::string key = res[1].str();\n"
						"\t\t\t\t\t\tread_item_S new_item;\n"
						"\t\t\t\t\t\tm_lookup.under[key] = new_item;\n"
						"\t\t\t\t\t\tcurrent_lookup = &m_lookup.under[key];\n"
						"\t\t\t\t\t\tfmt::println(\"[{{}}]\", key);\n"
						"\t\t\t\t\t\t}}\n"
						"\n"
						"\t\t\t\t\tbreak;\n"
						"\t\t\t\tcase ' ':\n"
						"\t\t\t\t\t// space as first character is a \"continue previous line\" thing... (but not implemented currently)\n"
						"\t\t\t\t\tbreak;\n"
						"\t\t\t\tdefault:\n"
						"\t\t\t\t\tregex = \"(\\\\w+) (.+)\";\n"
						"\t\t\t\t\tif (std::regex_search(line, res, regex) == true)\n"
						"\t\t\t\t\t\t{{\n"
						"\t\t\t\t\t\tstd::string key = res[1].str();\n"
						"\t\t\t\t\t\tstd::string value = res[2].str();\n"
						"\t\t\t\t\t\tcurrent_lookup->items[key] = value;\n"
						"\t\t\t\t\t\tfmt::println(\"{{}} = {{}}\", key, value);\n"
						"\t\t\t\t\t\t}}\n"
						"\t\t\t\t\tbreak;\n"
						"\t\t\t\t}}\n"
						"\t\t\t}}\n"
						"\t\tstd::fclose(file);\n"
						"\n"
						"\t\trv = do_rd_{1}(\"\", struct_to_fill);\n"
						"\t\t}}\n"
						"\treturn rv;\n"
						"\t}}\n", reader_class_name, s.name);
					}
				}

			fmt::println(f_h, "\tprotected:");

			for (auto &pair : structs)
				{
				const struct_S &s = pair.second;
				rv &= process_struct_reader(s, reader_class_name, f_cpp, f_h);
				}

			fmt::println(f_h,
				"\t}};\n");
			fmt::println(f_cpp,
				"\n");

			// writer class ===============================================================
			fmt::println(f_h,
				"class {0} : public {1}\n"
				"\t{{\n"
				"\tprivate:\n"
				"\t\tbool m_delta_only {{}};\n"
				"\t\tstd::FILE *m_file {{nullptr}};\n"
				"\tpublic:\n"
				"\t\t{0}(bool delta_only = true);",
				writer_class_name, reader_class_name);

			fmt::println(f_cpp,
				"\n"
				"{0}::{0}(bool delta_only) : {1}()\n"
				"{{\n"
				"\tm_delta_only = delta_only;\n"
				"}}\n",
				writer_class_name, reader_class_name);

			for (auto &pair : structs)
				{
				const struct_S &s = pair.second;
				if (s.incoming_count == 0)
					{
					fmt::println(f_h, "\t\tvirtual bool write_to_file(std::string const &filename, {} &struct_to_read);", s.name);

					fmt::println(f_cpp,
						"bool {0}::write_to_file(std::string const &filename, {1} &struct_to_read)\n"
						"\t{{\n"
						"\tbool rv{{}};\n"
						"\tm_file = std::fopen(filename.c_str(), \"w\");\n"
						"\tif (m_file != nullptr)\n"
						"\t\t{{\n"
						"\t\trv = do_wr_{1}(\"\", &struct_to_read);\n"
						"\t\tstd::fclose(m_file);\n"
						"\t\t}}\n"
						"\treturn rv;\n"
						"\t}}\n", writer_class_name, s.name);
					}
				}

			fmt::println(f_h, "\tprotected:");

			for (auto &pair : structs)
				{
				const struct_S &s = pair.second;
				rv &= process_struct_writer(s, writer_class_name, f_cpp, f_h);
				}

			fmt::println(f_h,
				"\t}};");
			fmt::println(f_cpp,
				"\n");

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

bool process_items_ini_C::process_struct_reader(struct_S const &s, std::string const class_name, std::FILE *out_file_cpp, std::FILE *out_file_h)
	{
	return false;
	}

bool process_items_ini_C::process_struct_writer(struct_S const &s, std::string const class_name, std::FILE *out_file_cpp, std::FILE *out_file_h)
	{
	return false;
	}

bool process_items_n_ini_C::process_struct_reader(struct_S const &s, std::string const class_name, std::FILE *out_file_cpp, std::FILE *out_file_h)
	{
	bool rv{ true };

	fmt::println(out_file_h, "\t\tbool do_rd_{0}(std::string path, {0} &data);", s.name);

	fmt::println(out_file_cpp,
		"bool {0}::do_rd_{1}(std::string path, {1} &data)\n"
		"\t{{\n"
		, class_name, s.name);
	fmt::println(out_file_cpp, "\tbool rv = true;");
	fmt::println(out_file_cpp, "\ttry");
	fmt::println(out_file_cpp, "\t\t{{");
	if (s.simple.empty() == false)
		{
		fmt::println(out_file_cpp, "\t\tauto found_items = m_lookup.under.find(path);");
		fmt::println(out_file_cpp, "\t\tif (found_items != m_lookup.under.cend())");
		fmt::println(out_file_cpp, "\t\t\t{{");
		fmt::println(out_file_cpp, "\t\t\tauto items = found_items->second.items;");
		for (auto &sim : s.simple)
			{
			std::string convert{};
			switch (sim.line_type)
				{
				case simple_item_type_E::bool_E:
					break;
				case simple_item_type_E::int_E:
				case simple_item_type_E::unsigned_int_E:
					convert = "std::stoi";
					break;
				case simple_item_type_E::float_E:
					convert = "std::stof";
					break;
				case simple_item_type_E::double_E:
					convert = "std::stoi";
					break;
				case simple_item_type_E::long_E:
				case simple_item_type_E::short_E:
				case simple_item_type_E::unsigned_short_E:
				case simple_item_type_E::std_string_E:
				case simple_item_type_E::unsigned_long_E:
					break;

				default:
					fmt::println(out_file_cpp, "\t// WARNING FAILED TO PROCESS {}\n", sim.name);
					fmt::println("WARNING : FAILED TO PROCESS {}", sim.name);
					break;
				}
			fmt::println(out_file_cpp,
				"\t\t\tauto {0} = items.find(\"{0}\");\n"
				"\t\t\tif({0} != items.cend())\n"
				"\t\t\t\t{{\n"
				"\t\t\t\tdata.{0} = {1}({0}->second);\n"
				"\t\t\t\t}}"
				, sim.name, convert);
			}
		for (auto &co : s.complex)
			{
			complex_item_type_E line_type = co.line_type;

			switch (line_type)
				{
				case complex_item_type_E::enum_E:
					fmt::println(out_file_cpp,
						"\t\t\tauto {0} = items.find(\"{0}\");\n"
						"\t\t\tif({0} != items.cend())\n"
						"\t\t\t\t{{\n"
						"\t\t\t\tstfx::s_to_e(({0}->second, data.{0});\n"
						"\t\t\t\t}}"
						, co.name);
					break;
				default:
					break;
				}
			}
		fmt::println(out_file_cpp, "\t\t\t}}");
		}
	bool has_children{ false };
	for (auto &co : s.complex)
		{
		complex_item_type_E line_type = co.line_type;

		switch (line_type)
			{
			case complex_item_type_E::struct_E:
			case complex_item_type_E::vector_E:
				has_children = true;
				break;
			default:
				break;
			}
		}
	if (has_children == true)
		{
		fmt::println(out_file_cpp,
			"\t\tif (path.length() > 0)\n"
			"\t\t\tpath += \".\";");
		}
	if (s.complex.empty() == false)
		{
		for (auto &co : s.complex)
			{
			complex_item_type_E line_type = co.line_type;

			switch (line_type)
				{
				case complex_item_type_E::enum_E:
					break;
				case complex_item_type_E::struct_E:
					fmt::println(out_file_cpp,
						"\t\trv = do_rd_{1}(path + \"{0}\", data.{0});"
						, co.name, co.type_name);
					break;
				case complex_item_type_E::vector_E:
					fmt::println(out_file_cpp, "// vector {}\t within structure NOT SUPPORTED by n_ini", co.name);
					fmt::println("// vector {}\t within structure NOT SUPPORTED by n_ini", co.name);
					rv = false;
					break;
				case complex_item_type_E::enum_or_struct_E:
					fmt::println(out_file_cpp, "// enum_or_struct_E {}\t NEEDS TO BE FIXED!!", co.name);
					fmt::println("// enum_or_struct_E {}\t NEEDS TO BE FIXED!!", co.name);
					rv = false;
					break;
				default:
					fmt::println(out_file_cpp, "\t// WARNING FAILED TO PROCESS {}", co.name);
					fmt::println("\t// WARNING FAILED TO PROCESS {}", co.name);
					rv = false;
					break;
				}
			}
		}

	fmt::println(out_file_cpp, "\t\t}}");

	fmt::println(out_file_cpp, "\tcatch (std::exception &)");
	fmt::println(out_file_cpp, "\t\t{{");
	fmt::println(out_file_cpp, "\t\trv = false;");
	fmt::println(out_file_cpp, "\t\t}}");
	fmt::println(out_file_cpp,
		"\treturn rv;\n"
		"\t}};\n");
	return rv;
	}

bool process_items_n_ini_C::process_struct_writer(struct_S const &s, std::string const class_name, std::FILE *out_file_cpp, std::FILE *out_file_h)
	{
	bool rv{ true };

	fmt::println(out_file_h, "\t\tbool do_wr_{0}(std::string path, {0} *data);", s.name);

	fmt::println(out_file_cpp,
		"bool {0}::do_wr_{1}(std::string path, {1} *data)\n"
		"\t{{\n"
		, class_name, s.name);
	fmt::println(out_file_cpp, "\tbool rv = true;");
	fmt::println(out_file_cpp, "\tstd::string lower_path;");
	fmt::println(out_file_cpp, "\t{0} default_data;", s.name);
	fmt::println(out_file_cpp,
		"\tif (path.length() > 0)\n"
		"\t\t{{\n"
		"\t\tfmt::println(m_file, \"[{{}}]\", path);\n"
		"\t\t}}\n"
	);

	for (auto &sim : s.simple)
		{
		switch (sim.line_type)
			{
			case simple_item_type_E::bool_E:
			case simple_item_type_E::int_E:
			case simple_item_type_E::unsigned_int_E:
			case simple_item_type_E::float_E:
			case simple_item_type_E::double_E:
			case simple_item_type_E::long_E:
			case simple_item_type_E::short_E:
			case simple_item_type_E::unsigned_short_E:
			case simple_item_type_E::std_string_E:
			case simple_item_type_E::unsigned_long_E:
				fmt::println(out_file_cpp,
					"\tif(m_delta_only == false || data->{0} != default_data.{0})\n"
					"\t\t{{\n"
					"\t\tfmt::println(m_file, \"{0} {{}}\", data->{0});\n"
					"\t\t}}"
					, sim.name);
				break;

			default:
				fmt::println(out_file_cpp, "\t// WARNING FAILED TO PROCESS {}\n", sim.name);
				fmt::println("WARNING : FAILED TO PROCESS {}", sim.name);
				break;
			}
		}
	bool has_children{ false };
	for (auto &co : s.complex)
		{
		complex_item_type_E line_type = co.line_type;

		switch (line_type)
			{
			case complex_item_type_E::struct_E:
			case complex_item_type_E::vector_E:
				has_children = true;
				break;
			default:
				break;
			}
		}
	for (auto &co : s.complex)
		{
		complex_item_type_E line_type = co.line_type;

		switch (line_type)
			{
			case complex_item_type_E::enum_E:
				fmt::println(out_file_cpp,
					"\tif(m_delta_only == false || data->{0} != default_data.{0})\n"
					"\t\t{{\n"
					"\t\tfmt::println(m_file, \"{0} {{}}\", data->{0});\n"
					"\t\t}}"
					, co.name);
				break;
			case complex_item_type_E::struct_E:
				fmt::println(out_file_cpp,
					"\tlower_path = path;\n"
					"\tif(lower_path.length() > 0)\n"
					"\t{{\n"
					"\t\tlower_path += \".\";\t\t//separator\n"
					"\t}}\n"
					"\tlower_path += \"{0}\";\n"
					"\tdo_wr_{1}(lower_path, &data->{0});\n"
					, co.name, co.type_name);
				break;
			case complex_item_type_E::vector_E:
				fmt::println(out_file_cpp, "// vector {}\t within structure NOT SUPPORTED by n_ini", co.name);
				fmt::println("// vector {}\t within structure NOT SUPPORTED by n_ini", co.name);
				rv = false;
				break;
			case complex_item_type_E::enum_or_struct_E:
				fmt::println(out_file_cpp, "// enum_or_struct_E {}\t NEEDS TO BE FIXED!!", co.name);
				fmt::println("// enum_or_struct_E {}\t NEEDS TO BE FIXED!!", co.name);
				rv = false;
				break;
			default:
				fmt::println(out_file_cpp, "\t// WARNING FAILED TO PROCESS {}", co.name);
				fmt::println("\t// WARNING FAILED TO PROCESS {}", co.name);
				rv = false;
				break;
			}
		}
	fmt::println(out_file_cpp,
		"\treturn rv;\n"
		"\t}};\n");

	return rv;
	}
