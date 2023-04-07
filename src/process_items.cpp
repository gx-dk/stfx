//process_items.cpp

#include "process_items.h"

#include <chrono>
#include <cstdio>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include "items.h"

bool process_items_C::process_items(info_items_C &items, std::string in_filename)
	{
	bool rv{ false };

	auto time = std::chrono::system_clock::now();
	m_timestamp = fmt::format("{:%Y-%m-%d %H:%M:%S}", time);

	rv = fixup_types_of_names(items);
	rv &= process_all_enums(items, in_filename, "test_out/enums");
	rv &= process_all_structs(items, in_filename, "test_out/structs");
	return rv;
	}

// look though all items in the struct dictionary ... and fixup enum_or_struct to the correct type!!
bool process_items_C::fixup_types_of_names(info_items_C &items)
	{
	bool rv {true};
	auto &structs = items.get_structs();
	for (auto &pair : structs)
		{
		struct_S &s = pair.second;
		for (struct_line_complex_S &co : s.complex)
			{
			complex_item_type_E line_type = co.line_type;
			if (line_type == complex_item_type_E::enum_or_struct_E)
				{
				co.line_type = items.get_item_type(co.type_name);
				if (co.line_type == complex_item_type_E::unknown_E)
					rv = false;
				}
			}
		}
	return rv;
	}



bool process_items_C::process_all_enums(info_items_C & items, std::string in_filename, std::string out_filename_root)
	{
	bool rv {true};
	std::string out_filename_cpp = out_filename_root + ".cpp";
	std::string out_filename_h = out_filename_root + ".h";
	auto &enums = items.get_enums();

	std::FILE *f_cpp;
	f_cpp = std::fopen(out_filename_cpp.c_str(), "w");
	if (f_cpp != nullptr)
		{
		std::FILE* f_h;
		f_h = std::fopen(out_filename_h.c_str(), "w");
		if (f_h != nullptr)
			{
			rv = true;		// files are open

			fmt::print(f_cpp, "// {0}\n// created {1}\n\n", out_filename_cpp, m_timestamp);
			fmt::print(f_cpp, "#include \"{}\"\n\n", out_filename_h);
			fmt::print(f_cpp, "#include <string>\n#include <map>\n\n");
			fmt::print(f_cpp, "#include \"{}\"\n\n", in_filename);

			fmt::print(f_h, "// {0}\n// created {1}\n\n", out_filename_h, m_timestamp);
			fmt::print(f_h, "#pragma once\n\n");
			fmt::print(f_h, "#include <string>\n\n");
			fmt::print(f_h, "#include <fmt/format.h>\n\n");
			fmt::print(f_h, "#include \"{}\"\n\n", in_filename);
			fmt::print(f_h, "namespace stfx\n\t{{\n");	// start namespace ! 

			for (auto& pair : enums)
				{
				const enum_S& e = pair.second;
				process_enum(e, in_filename, f_cpp, f_h);
				}

			fmt::print(f_h, "\t}};\n");		// finish namespace

			// now generate fmt::formatter items
			for (auto& pair : enums)
				{
				const enum_S& e = pair.second;
				create_fmt_templates_for_enum(e, f_h);
				}


			std::fclose(f_h);
			}
		else
			{
			fmt::print("\nERROR : Failed to open output file {}\n", out_filename_h);
			}
		std::fclose(f_cpp);
		}
	else
		{
		fmt::print("\nERROR : Failed to open output file {}\n", out_filename_cpp);
		}
	return rv;
	}

bool process_items_C::process_enum(const enum_S &the_enum, std::string in_filename, std::FILE* out_file_cpp, std::FILE* out_file_h)
	{
	// s_from_e

	fmt::print(out_file_h, "\tconst std::string s_from_e({0} e);\n", the_enum.name);

	fmt::print(out_file_cpp, "const std::string stfx::s_from_e({0} e)\n\t{{\n", the_enum.name);

	fmt::print(out_file_cpp, "\tstd::map < {0}, std::string > {0}_to_string =\n\t\t{{\n", the_enum.name);

	for (auto &ent : the_enum.enums)
		{
		if (the_enum.is_class_enum == true)
			{
			fmt::print(out_file_cpp, "\t\t\t{{ {0}::{1}, \"{1}\" }},\n", the_enum.name, ent.name);
			}
		else
			{
			fmt::print(out_file_cpp, "\t\t\t{{ {0}, \"{0}\" }},\n", ent.name);
			}
		}
	fmt::print(out_file_cpp, "\t\t}};\n");
	fmt::print(out_file_cpp, "\tstd::string rv = ({0}_to_string.find(e) != {0}_to_string.end()) ? {0}_to_string[e] : std::to_string(int(e));\n", the_enum.name);
	fmt::print(out_file_cpp, "\treturn rv;\n\t}};\n\n");

	// s_to_e

	fmt::print(out_file_h, "\tbool s_to_e(std::string s, {} &e);\n", the_enum.name);

	fmt::print(out_file_cpp, "bool stfx::s_to_e(std::string s, {} &e)\n\t{{\n", the_enum.name);
	fmt::print(out_file_cpp, "\tbool rv {{false}};\n");

	fmt::print(out_file_cpp, "\tstd::map < std::string, {0} > string_to_{0} =\n\t\t{{\n", the_enum.name);

	for (auto &ent : the_enum.enums)
		{
		if (the_enum.is_class_enum == true)
			{
			fmt::print(out_file_cpp, "\t\t\t{{ \"{1}\", {0}::{1} }},\n", the_enum.name, ent.name);
			}
		else
			{
			fmt::print(out_file_cpp, "\t\t\t{{ \"{0}\", {0} }},\n", ent.name);
			}
		}
	fmt::print(out_file_cpp, "\t\t}};\n");
	fmt::print(out_file_cpp,
			"\tif(string_to_{0}.find(s) != string_to_{0}.end())\n"
				"\t\t{{\n"
				"\t\te = string_to_{0}[s];\n"
				"\t\trv = true;\n"
				"\t\t}}\n"
			"\telse\n"
				"\t\t{{\n"
				"\t\tint i = std::stoi(s);	// give this rubbish ... and get an invalid_argument exception ;-)\n"
				"\t\te = {0}(i);\n"
				"\t\t// note rv NOT set to true ! \n"
				"\t\t}}\n", the_enum.name);

	fmt::print(out_file_cpp, "\treturn rv;\n\t}};\n\n");
	return true;
	}

bool process_items_C::create_fmt_templates_for_enum(const enum_S& the_enum, std::FILE* out_file_h)
	{
	fmt::print(out_file_h,
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
				"\t}};\n", the_enum.name);
			return true;
	}

bool process_items_C::process_all_structs(info_items_C &items, std::string in_filename, std::string out_filename_root)
	{
	bool rv {false};
	std::string const out_filename_cpp = out_filename_root + ".cpp";
	std::string const out_filename_h = out_filename_root + ".h";
	std::string class_name = "xml_reader";
	auto &structs = items.get_structs();

	std::FILE* f_cpp;
	f_cpp = std::fopen(out_filename_cpp.c_str(), "w");
	if (f_cpp != nullptr)
		{
		std::FILE* f_h;
		f_h = std::fopen(out_filename_h.c_str(), "w");
		if (f_h != nullptr)
			{
			rv = true;

			fmt::print(f_cpp, "// {0}\n// created {1}\n\n", out_filename_cpp, m_timestamp);
			fmt::print(f_cpp, "#include \"{}\"\n\n", out_filename_h);
			fmt::print(f_cpp, "#include <string>\n"
				"#include <map>\n"
				"#include <stdexcept>\n"
				"\n");
			fmt::print(f_cpp, "#include <tinyxml2.h>\n"
				"#include <tixml2ex.h>\n\n");
			fmt::print(f_cpp, "#include \"enums.h\"\n"
				"#include \"{}\"\n\n", in_filename);

			fmt::print(f_h, "// {0}\n// created {1}\n\n", out_filename_h, m_timestamp);
			fmt::print(f_h,
				"#pragma once\n"
				"\n"
				"#include <string>\n"
				"#include <tinyxml2.h>\n"
				"#include \"{0}\"\n"
				"\n", in_filename);

			// reader class ===============================================================
			fmt::print(f_h,
				"class {0}\n"
				"\t{{\n"
				"\tprivate:\n"
				"\t\tstd::string m_xml_file {{}};\n"
				"\tpublic:\n"
				"\t\t{0}(std::string xml_file);\n", class_name);

			fmt::print(f_cpp,
				"\n"
				"{0}::{0}(std::string xml_file)\n"
				"{{\n"
				"\tm_xml_file = xml_file;\n"
				"}}\n"
				"\n", class_name);

			for (auto& pair : structs)
				{
				const struct_S& s = pair.second;
				rv &= process_struct_reader(s, class_name, f_cpp, f_h);
				}

			fmt::print(f_h,
				"\n"
				"\t}};\n");
			fmt::print(f_cpp,
				"\n\n");

			// writer class ===============================================================
			class_name = "xml_writer";
			fmt::print(f_h,
				"class {0}\n"
				"\t{{\n"
				"\tprivate:\n"
				"\t\tstd::string m_xml_file {{}};\n"
				"\t\tbool m_delta_only {{}};\n"
				"\tpublic:\n"
				"\t\t{0}(std::string xml_file, bool delta_only = true);\n", class_name);

			fmt::print(f_cpp,
				"\n"
				"{0}::{0}(std::string xml_file, bool delta_only)\n"
				"{{\n"
				"\tm_xml_file = xml_file;\n"
				"\tm_delta_only = delta_only;\n"
				"}}\n"
				"\n", class_name);

			for (auto& pair : structs)
				{
				const struct_S& s = pair.second;
				rv &= process_struct_writer(s, class_name, f_cpp, f_h);
				}

			fmt::print(f_h,
				"\n"
				"\t}};\n");
			fmt::print(f_cpp,
				"\n\n");

			std::fclose(f_h);
			}
		else
			{
			fmt::print("\nERROR : Failed to open output file {}\n", out_filename_h);
			}
		std::fclose(f_cpp);
		}
	else
		{
		fmt::print("\nERROR : Failed to open output file {}\n", out_filename_cpp);
		}

	return rv;
	}

bool process_items_C::process_struct_reader(struct_S const& s, std::string const class_name, std::FILE* out_file_cpp, std::FILE* out_file_h)
	{
	bool rv {true};

	fmt::print(out_file_h,	"\t\tbool do_{0}(tinyxml2::XMLElement *el, {0} *data);\n", s.name);

	fmt::print(out_file_cpp,
			"bool {0}::do_{1}(tinyxml2::XMLElement *el, {1} *data)\n"
			"\t{{\n", class_name, s.name);
	fmt::print(out_file_cpp,	"\tbool rv = true;\n");

	for (auto &sim : s.simple)
		{
		switch (sim.line_type)
			{
			case simple_item_type_E::bool_E:
			case simple_item_type_E::int_E:
			case simple_item_type_E::unsigned_int_E:
			case simple_item_type_E::float_E:
			case simple_item_type_E::double_E:
				fmt::print(out_file_cpp, "\tel->QueryAttribute(\"{0}\", &data->{0});\n", sim.name);
			break;
			case simple_item_type_E::std_string_E:
				fmt::print(out_file_cpp,
						"\t\t{{\n"
						"\t\tconst char *pt;\n"
						"\t\tif (tinyxml2::XML_SUCCESS == el->QueryAttribute(\"{0}\", &pt))\n"
						"\t\t\tdata->{0} = pt;\n"
						"\t\t}}\n", sim.name);
				break;
			default:
				fmt::print(out_file_cpp, "\t// WARNING FAILED TO PROCESS {}\n", sim.name);
				break;
			}
		}

	bool has_children{ false };
	for (auto& co : s.complex)
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
		fmt::print(out_file_cpp, "\ttinyxml2::XMLElement *ch_el;\n");
		}
	for (auto &co : s.complex)
		{
		complex_item_type_E line_type = co.line_type;

		switch (line_type)
			{
			case complex_item_type_E::enum_E:
				fmt::print(out_file_cpp, 
						"\t\t{{\n"
						"\t\tconst char *pt;\n"
						"\t\tif (tinyxml2::XML_SUCCESS == el->QueryAttribute(\"{0}\", &pt))\n"
						"\t\t\tstfx::s_to_e(pt, data->{0});\n"
						"\t\t}}\n", co.name);
				break;
			case complex_item_type_E::struct_E:
				fmt::print(out_file_cpp,
						"\tch_el = el->FirstChildElement(\"{0}\");\n"
						"\tif (ch_el != nullptr)\n"
						"\t\t{{\n"
						"\t\tdo_{1}(ch_el, &data->{0});\n"
						"\t\t}}\n", co.name, co.type_name);
				break;
			case complex_item_type_E::vector_E:
				fmt::print(out_file_cpp,
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
						"\t\t}}\n", co.name, co.type_name);
				break;
			case complex_item_type_E::enum_or_struct_E:
				fmt::print(out_file_cpp, "// enum_or_struct_E {}\t NEEDS TO BE FIXED!!\n", co.name);
				fmt::print("// enum_or_struct_E {}\t NEEDS TO BE FIXED!!\n", co.name);
				rv = false;
				break;
			default:
				fmt::print(out_file_cpp, "\t// WARNING FAILED TO PROCESS {}\n", co.name);
				fmt::print("\t// WARNING FAILED TO PROCESS {}\n", co.name);
				rv = false;
				break;
			}
		}
	fmt::print(out_file_cpp,
			"\treturn rv;\n"
			"\t}};\n"
			"\n");

	return rv;
	}

bool process_items_C::process_struct_writer(struct_S const& s, std::string const class_name, std::FILE* out_file_cpp, std::FILE* out_file_h)
	{
	bool rv{ true };

	fmt::print(out_file_h, "\t\tbool do_{0}(tinyxml2::XMLElement *el, {0} *data);\n", s.name);

	fmt::print(out_file_cpp,
		"bool {0}::do_{1}(tinyxml2::XMLElement *el, {1} *data)\n"
		"\t{{\n", class_name, s.name);
	fmt::print(out_file_cpp, "\tbool rv = true;\n");
	fmt::print(out_file_cpp, "\t{0} default_data;\n", s.name);

	for (auto& sim : s.simple)
		{
		switch (sim.line_type)
			{
			case simple_item_type_E::bool_E:
			case simple_item_type_E::int_E:
			case simple_item_type_E::unsigned_int_E:
			case simple_item_type_E::float_E:
			case simple_item_type_E::double_E:
				fmt::print(out_file_cpp,
					"\tif(m_delta_only == false || data->{0} != default_data.{0})\n"
					"\t\tel->SetAttribute(\"{0}\", data->{0});\n", sim.name);
				break;
			case simple_item_type_E::std_string_E:
				fmt::print(out_file_cpp,
					"\tif(m_delta_only == false || data->{0} != default_data.{0})\n"
					"\t\tel->SetAttribute(\"{0}\", data->{0}.c_str());\n", sim.name);
				break;
			default:
				fmt::print(out_file_cpp, "\t// WARNING FAILED TO PROCESS {}\n", sim.name);
				break;
			}
		}
	bool has_children{ false };
	for (auto& co : s.complex)
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
		fmt::print(out_file_cpp, "\ttinyxml2::XMLElement *ch_el;\n");
		}
	for (auto& co : s.complex)
		{
		complex_item_type_E line_type = co.line_type;

		switch (line_type)
			{
			case complex_item_type_E::enum_E:
				fmt::print(out_file_cpp,
					"\tif(m_delta_only == false || data->{0} != default_data.{0})\n"
					"\t\tel->SetAttribute(\"{0}\", stfx::s_from_e(data->{0}).c_str());\n", co.name);
				break;
			case complex_item_type_E::struct_E:
				fmt::print(out_file_cpp,
					"\tch_el = el->InsertNewChildElement(\"{0}\");\n"
					"\tif (ch_el != nullptr)\n"
					"\t\t{{\n"
					"\t\tdo_{1}(ch_el, &data->{0});\n"
					"\t\t}}\n", co.name, co.type_name);
				break;
			case complex_item_type_E::vector_E:
				fmt::print(out_file_cpp,
					"\tch_el = el->InsertNewChildElement(\"{0}\");\n"
					"\tif (ch_el != nullptr)\n"
					"\t\t{{\n"
					"\t\tfor ({1} itr : data->{0})\n"
					"\t\t\t{{\n"
					"\t\t\ttinyxml2::XMLElement *ch_ch_el = ch_el->InsertNewChildElement(\"{1}\");\n"
					"\t\t\tdo_{1}(ch_ch_el, &itr);\n"
					"\t\t\t}}\n"
					"\t\t}}\n", co.name, co.type_name);
				break;
			case complex_item_type_E::enum_or_struct_E:
				fmt::print(out_file_cpp, "// enum_or_struct_E {}\t NEEDS TO BE FIXED!!\n", co.name);
				fmt::print("// enum_or_struct_E {}\t NEEDS TO BE FIXED!!\n", co.name);
				rv = false;
				break;
			default:
				fmt::print(out_file_cpp, "\t// WARNING FAILED TO PROCESS {}\n", co.name);
				fmt::print("\t// WARNING FAILED TO PROCESS {}\n", co.name);
				rv = false;
				break;
			}
		}
	fmt::print(out_file_cpp,
		"\treturn rv;\n"
		"\t}};\n"
		"\n");

	return rv;
	}
