//process_items.cpp

#include "process_items.h"

#include <chrono>
#include <cstdio>
#include <filesystem>
#include <string>

#include <fmt/format.h>
#include <fmt/chrono.h>

#include "items.h"
#include "config_data.h"

std::string made_by{ "// Automatically generated using stfx. Do not directly edit this file, use stfx to re-create this file.\n// Licence : MIT License\n" };

bool process_items_C::process_items(info_items_C &items, const std::vector<std::string>&input_files, const output_spec &output)
	{
	bool rv{ false };

	auto time = std::chrono::system_clock::now();
	m_timestamp = fmt::format("{:%Y-%m-%d %H:%M:%S}", time);

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
		std::FILE *f_h;
		f_h = std::fopen(out_pathfilename_h.c_str(), "w");
		if (f_h != nullptr)
			{
			rv = true;		// files are open

			fmt::println(f_cpp, "// {}\n// created {}", out_filename_cpp, m_timestamp);
			fmt::println(f_cpp, "{}", made_by);
			fmt::println(f_cpp, "#include \"{}\"\n", out_filename_h);
			fmt::println(f_cpp, "#include <string>\n#include <map>\n");
			for (const auto &in_filename : input_files)
			{
				fmt::println(f_cpp, "#include \"{}\"", in_filename);
			}
			fmt::println(f_cpp, "");

			fmt::println(f_h, "// {0}\n// created {1}", out_filename_h, m_timestamp);
			fmt::println(f_h, "{}", made_by);
			fmt::println(f_h, "#pragma once\n");
			fmt::println(f_h, "#include <string>");
			fmt::println(f_h, "#include <fmt/format.h>\n");
			for (const auto& in_filename : input_files)
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

bool process_items_C::process_all_structs(info_items_C &items, const std::vector<std::string> &input_files, const output_spec &output)
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
		std::FILE *f_h;
		f_h = std::fopen(out_pathfilename_h.c_str(), "w");
		if (f_h != nullptr)
			{
			rv = true;

			fmt::println(f_cpp, "// {0}\n// created {1}", out_filename_cpp, m_timestamp);
			fmt::println(f_cpp, "{}", made_by);
			fmt::println(f_cpp, "#include \"{}\"\n", out_filename_h);
			fmt::println(f_cpp, "#include <string>\n"
				"#include <map>\n"
				"#include <stdexcept>");
			fmt::println(f_cpp, "#include <tinyxml2.h>\n"
				"#include <tixml2ex.h>\n");
			if (output.enum_file.empty() || output.enum_file == "-")
			{
				fmt::println(f_cpp, "// not including blank enum file");
			}
			else
			{
				fmt::println(f_cpp, "#include \"{}\"", enums_filename_h);
			}
			for (const auto& in_filename : input_files)
			{
				fmt::println(f_cpp, "#include \"{}\"", in_filename);
			}

			fmt::println(f_h, "// {0}\n// created {1}", out_filename_h, m_timestamp);
			fmt::println(f_h, "{}", made_by);

			fmt::println(f_h,
				"#pragma once\n"
				"\n"
				"#include <string>\n"
				"#include <tinyxml2.h>\n");
			for (const auto& in_filename : input_files)
			{
				fmt::println(f_h, "#include \"{}\"", in_filename);
			}
			fmt::println(f_h, "");


			// reader class ===============================================================
			fmt::println(f_h,
				"class {0}\n"
				"\t{{\n"
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
						"\ttinyxml2::XMLError er;\n"
						"\ttinyxml2::XMLDocument doc;\n"
						"\n"
						"\ter = doc.LoadFile(filename.c_str());\n"
						"\tif (er == tinyxml2::XML_SUCCESS)\n"
						"\t\t{{\n"
						"\t\ttinyxml2::XMLElement* el = doc.RootElement();\n"
						"\t\trv = do_{1}(el, &struct_to_fill);\n"
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
						"\ttinyxml2::XMLError er;\n"
						"\ttinyxml2::XMLDocument doc_w;\n"
						"\tauto ch = doc_w.NewElement(\"{1}\");\n"
						"\tdoc_w.InsertFirstChild(ch);\n"
						"\ttinyxml2::XMLElement * el = doc_w.RootElement();\n"
						"\tdo_wr_{1}(el, &struct_to_read);\n"
						"\ter = doc_w.SaveFile(filename.c_str());\n"
						"\trv = (er == tinyxml2::XML_SUCCESS);\n"
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

bool process_items_C::process_struct_reader(struct_S const &s, std::string const class_name, std::FILE *out_file_cpp, std::FILE *out_file_h)
	{
	bool rv{ true };

	fmt::println(out_file_h, "\t\tbool do_{0}(tinyxml2::XMLElement *el, {0} *data);", s.name);

	fmt::println(out_file_cpp,
		"bool {0}::do_{1}(tinyxml2::XMLElement *el, {1} *data)\n"
		"\t{{",
		class_name, s.name);
	fmt::println(out_file_cpp, "\tbool rv = true;");

	for (auto &sim : s.simple)
		{
		switch (sim.line_type)
			{
			case simple_item_type_E::bool_E:
			case simple_item_type_E::int_E:
			case simple_item_type_E::unsigned_int_E:
			case simple_item_type_E::float_E:
			case simple_item_type_E::double_E:
				fmt::println(out_file_cpp, "\tel->QueryAttribute(\"{0}\", &data->{0});", sim.name);
				break;
			case simple_item_type_E::std_string_E:
				fmt::println(out_file_cpp,
					"\t\t{{\n"
					"\t\tconst char *pt;\n"
					"\t\tif (tinyxml2::XML_SUCCESS == el->QueryAttribute(\"{0}\", &pt))\n"
					"\t\t\tdata->{0} = pt;\n"
					"\t\t}}", sim.name);
				break;
			case simple_item_type_E::short_E:
				fmt::println(out_file_cpp, "\t{{ int i; el->QueryIntAttribute(\"{0}\", &i); data->{0} = short(i); }}", sim.name);
				break;
			case simple_item_type_E::unsigned_short_E:
				fmt::println(out_file_cpp, "\t{{ unsigned int i; el->QueryUnsignedAttribute(\"{0}\", &i); data->{0} = unsigned short(i); }}", sim.name);
				break;
			case simple_item_type_E::long_E:
				fmt::println(out_file_cpp, "\t{{ int i; el->QueryIntAttribute(\"{0}\", &i); data->{0} = long(i); }}", sim.name);
				break;
			case simple_item_type_E::unsigned_long_E:
				fmt::println(out_file_cpp, "\t{{ unsigned int i; el->QueryUnsignedAttribute(\"{0}\", &i); data->{0} = unsigned long(i); }}", sim.name);
				break;
			default:
				fmt::println(out_file_cpp, "\t// WARNING FAILED TO PROCESS {}", sim.name);
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
	if (has_children == true)
		{
		fmt::println(out_file_cpp, "\ttinyxml2::XMLElement *ch_el;");
		}
	for (auto &co : s.complex)
		{
		complex_item_type_E line_type = co.line_type;

		switch (line_type)
			{
			case complex_item_type_E::enum_E:
				fmt::println(out_file_cpp,
					"\t\t{{\n"
					"\t\tconst char *pt;\n"
					"\t\tif (tinyxml2::XML_SUCCESS == el->QueryAttribute(\"{0}\", &pt))\n"
					"\t\t\tstfx::s_to_e(pt, data->{0});\n"
					"\t\t}}", co.name);
				break;
			case complex_item_type_E::struct_E:
				fmt::println(out_file_cpp,
					"\tch_el = el->FirstChildElement(\"{0}\");\n"
					"\tif (ch_el != nullptr)\n"
					"\t\t{{\n"
					"\t\tdo_{1}(ch_el, &data->{0});\n"
					"\t\t}}", co.name, co.type_name);
				break;
			case complex_item_type_E::vector_E:
				fmt::println(out_file_cpp,
					"\tch_el = el->FirstChildElement(\"{0}\");\n"
					"\tif (ch_el != nullptr)\n"
					"\t\t{{\n"
					"\t\tfor (auto item : ch_el)\n"
					"\t\t\t{{\n"
					"\t\t\t{1} s;\n"
					"\t\t\tif (do_{1}(item, &s) == true)\n"
					"\t\t\t\t{{\n"
					"\t\t\t\tdata->{0}.push_back(s);\n"
					"\t\t\t\t}}\n"
					"\t\t\t}}\n"
					"\t\t}}", co.name, co.type_name);
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

bool process_items_C::process_struct_writer(struct_S const &s, std::string const class_name, std::FILE *out_file_cpp, std::FILE *out_file_h)
	{
	bool rv{ true };

	fmt::println(out_file_h, "\t\tbool do_wr_{0}(tinyxml2::XMLElement *el, {0} *data);", s.name);

	fmt::println(out_file_cpp,
		"bool {0}::do_wr_{1}(tinyxml2::XMLElement *el, {1} *data)\n"
		"\t{{", class_name, s.name);
	fmt::println(out_file_cpp, "\tbool rv = true;");
	fmt::println(out_file_cpp, "\t{0} default_data;", s.name);

	for (auto &sim : s.simple)
		{
		switch (sim.line_type)
			{
			case simple_item_type_E::bool_E:
			case simple_item_type_E::int_E:
			case simple_item_type_E::unsigned_int_E:
			case simple_item_type_E::float_E:
			case simple_item_type_E::double_E:
				fmt::println(out_file_cpp,
					"\tif(m_delta_only == false || data->{0} != default_data.{0})\n"
					"\t\tel->SetAttribute(\"{0}\", data->{0});", sim.name);
				break;
			case simple_item_type_E::std_string_E:
				fmt::println(out_file_cpp,
					"\tif(m_delta_only == false || data->{0} != default_data.{0})\n"
					"\t\tel->SetAttribute(\"{0}\", data->{0}.c_str());", sim.name);
				break;
			case simple_item_type_E::long_E:
			case simple_item_type_E::short_E:
				fmt::println(out_file_cpp,
					"\tif(m_delta_only == false || data->{0} != default_data.{0})\n"
					"\t\tel->SetAttribute(\"{0}\", int(data->{0}));", sim.name);
				break;
			case simple_item_type_E::unsigned_long_E:
			case simple_item_type_E::unsigned_short_E:
				fmt::println(out_file_cpp,
					"\tif(m_delta_only == false || data->{0} != default_data.{0})\n"
					"\t\tel->SetAttribute(\"{0}\", unsigned(data->{0}));", sim.name);
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
	if (has_children == true)
		{
		fmt::println(out_file_cpp, "\ttinyxml2::XMLElement *ch_el;");
		}
	for (auto &co : s.complex)
		{
		complex_item_type_E line_type = co.line_type;

		switch (line_type)
			{
			case complex_item_type_E::enum_E:
				fmt::println(out_file_cpp,
					"\tif(m_delta_only == false || data->{0} != default_data.{0})\n"
					"\t\tel->SetAttribute(\"{0}\", stfx::s_from_e(data->{0}).c_str());", co.name);
				break;
			case complex_item_type_E::struct_E:
				fmt::println(out_file_cpp,
					"\tch_el = el->InsertNewChildElement(\"{0}\");\n"
					"\tif (ch_el != nullptr)\n"
					"\t\t{{\n"
					"\t\tdo_wr_{1}(ch_el, &data->{0});\n"
					"\t\t}}", co.name, co.type_name);
				break;
			case complex_item_type_E::vector_E:
				fmt::println(out_file_cpp,
					"\tch_el = el->InsertNewChildElement(\"{0}\");\n"
					"\tif (ch_el != nullptr)\n"
					"\t\t{{\n"
					"\t\tfor ({1} itr : data->{0})\n"
					"\t\t\t{{\n"
					"\t\t\ttinyxml2::XMLElement *ch_ch_el = ch_el->InsertNewChildElement(\"{1}\");\n"
					"\t\t\tdo_wr_{1}(ch_ch_el, &itr);\n"
					"\t\t\t}}\n"
					"\t\t}}", co.name, co.type_name);
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
