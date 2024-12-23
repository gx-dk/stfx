//process_items_xml_attrib.cpp

#include "process_items_xml.h"

#include <chrono>
#include <cstdio>
#include <filesystem>
#include <string>

#include <fmt/format.h>
#include <fmt/chrono.h>

#include "items.h"
#include "config_data.h"

bool process_items_xml_attrib_C::process_struct_reader(struct_S const &s, std::string const class_name, std::FILE *out_file_cpp, std::FILE *out_file_h)
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

bool process_items_xml_attrib_C::process_struct_writer(struct_S const &s, std::string const class_name, std::FILE *out_file_cpp, std::FILE *out_file_h, bool no_special_delta)
	{
	bool rv{ true };

	fmt::println(out_file_h, "\t\tbool do_wr_{0}(tinyxml2::XMLElement *el, {0} *data);", s.name);

	fmt::println(out_file_cpp,
		"bool {0}::do_wr_{1}(tinyxml2::XMLElement *el, {1} *data)\n"
		"\t{{", class_name, s.name);
	fmt::println(out_file_cpp, "\tbool rv = true;");
	if (no_special_delta == false)
		{
		fmt::println(out_file_cpp, "\t{0} default_data;", s.name);
		}

	for (auto &sim : s.simple)
		{
		switch (sim.line_type)
			{
			case simple_item_type_E::bool_E:
			case simple_item_type_E::int_E:
			case simple_item_type_E::unsigned_int_E:
			case simple_item_type_E::float_E:
			case simple_item_type_E::double_E:
				if (no_special_delta == true)
					{
					fmt::println(out_file_cpp,
						"\tel->SetAttribute(\"{0}\", data->{0});", sim.name);
					}
				else
					{
					fmt::println(out_file_cpp,
						"\tif(m_delta_only == false || data->{0} != default_data.{0})\n"
						"\t\tel->SetAttribute(\"{0}\", data->{0});", sim.name);
					}
				break;
			case simple_item_type_E::std_string_E:
				if (no_special_delta == true)
					{
					fmt::println(out_file_cpp,
						"\tel->SetAttribute(\"{0}\", data->{0}.c_str());", sim.name);
					}
				else
					{
					fmt::println(out_file_cpp,
						"\tif(m_delta_only == false || data->{0} != default_data.{0})\n"
						"\t\tel->SetAttribute(\"{0}\", data->{0}.c_str());", sim.name);
					}
				break;
			case simple_item_type_E::long_E:
			case simple_item_type_E::short_E:
				if (no_special_delta == true)
					{
					fmt::println(out_file_cpp,
						"\tel->SetAttribute(\"{0}\", int(data->{0}));", sim.name);
					}
				else
					{
					fmt::println(out_file_cpp,
						"\tif(m_delta_only == false || data->{0} != default_data.{0})\n"
						"\t\tel->SetAttribute(\"{0}\", int(data->{0}));", sim.name);
					}
				break;
			case simple_item_type_E::unsigned_long_E:
			case simple_item_type_E::unsigned_short_E:
				if (no_special_delta == true)
					{
					fmt::println(out_file_cpp,
						"\tel->SetAttribute(\"{0}\", unsigned(data->{0}));", sim.name);
					}
				else
					{
					fmt::println(out_file_cpp,
						"\tif(m_delta_only == false || data->{0} != default_data.{0})\n"
						"\t\tel->SetAttribute(\"{0}\", unsigned(data->{0}));", sim.name);
					}
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
				if (no_special_delta == true)
					{
					fmt::println(out_file_cpp,
						"\tel->SetAttribute(\"{0}\", stfx::s_from_e(data->{0}).c_str());", co.name);
					}
				else
					{
					fmt::println(out_file_cpp,
						"\tif(m_delta_only == false || data->{0} != default_data.{0})\n"
						"\t\tel->SetAttribute(\"{0}\", stfx::s_from_e(data->{0}).c_str());", co.name);
					}
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
