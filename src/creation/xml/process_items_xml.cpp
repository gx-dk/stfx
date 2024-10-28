//process_items_xml.cpp

#include "process_items_xml.h"

#include <chrono>
#include <cstdio>
#include <filesystem>
#include <string>

#include <fmt/format.h>
#include <fmt/chrono.h>

#include "items.h"
#include "config_data.h"




bool process_items_xml_C::process_all_structs(info_items_C &items, const std::vector<std::string> &input_files, const output_spec &output)
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
		fmt::println("Opened xml struct .cpp file :\t{}", out_pathfilename_cpp);
		std::FILE *f_h;
		f_h = std::fopen(out_pathfilename_h.c_str(), "w");
		if (f_h != nullptr)
			{
			fmt::println("Opened xml struct .h file :\t{}", out_pathfilename_h);
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
				"#include <tinyxml2.h>\n");
			for (const auto &in_filename : input_files)
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



bool process_items_xml_C::process_struct_reader(struct_S const &s, std::string const class_name, std::FILE *out_file_cpp, std::FILE *out_file_h)
	{
	bool rv{ true };

	fmt::println(out_file_h, "\t\tbool do_{0}(tinyxml2::XMLElement *el, {0} *data);", s.name);

	fmt::println(out_file_cpp,
		"bool {0}::do_{1}(tinyxml2::XMLElement *el, {1} *data)\n"
		"\t{{\n"
		"\ttinyxml2::XMLElement *ch_el;\n"
		,
		class_name, s.name);
	fmt::println(out_file_cpp, "\tbool rv = true;");

	for (auto &sim : s.simple)
		{
		switch (sim.line_type)
			{
			case simple_item_type_E::bool_E:
				fmt::println(out_file_cpp,
					"\tch_el = find_element(el, \"{0}\");\n"
					"\tif(ch_el != nullptr)\n"
					"\t\tch_el->QueryBoolText(&data->{0});"
					, sim.name);
				break;
			case simple_item_type_E::int_E:
				fmt::println(out_file_cpp,
					"\tch_el = find_element(el, \"{0}\");\n"
					"\tif(ch_el != nullptr)\n"
					"\t\tch_el->QueryIntText(&data->{0});"
					, sim.name);
				break;
			case simple_item_type_E::unsigned_int_E:
				fmt::println(out_file_cpp,
					"\tch_el = find_element(el, \"{0}\");\n"
					"\tif(ch_el != nullptr)\n"
					"\t\tch_el->QueryUnsignedText(&data->{0});"
					, sim.name);
				break;
			case simple_item_type_E::float_E:
				fmt::println(out_file_cpp,
					"\tch_el = find_element(el, \"{0}\");\n"
					"\tif(ch_el != nullptr)\n"
					"\t\tch_el->QueryFloatText(&data->{0});"
					, sim.name);
				break;
			case simple_item_type_E::double_E:
				fmt::println(out_file_cpp,
					"\tch_el = find_element(el, \"{0}\");\n"
					"\tif(ch_el != nullptr)\n"
					"\t\tch_el->QueryDoubleText(&data->{0});"
					, sim.name);
				break;
			case simple_item_type_E::std_string_E:
				fmt::println(out_file_cpp,
					"\tch_el = find_element(el, \"{0}\");\n"
					"\tif(ch_el != nullptr)\n"
					"\t\t{{\n"
					"\t\tconst char *pt = ch_el->GetText();\n"
					"\t\tif(pt != nullptr)\n"
					"\t\t\tdata->{0} = pt;\n"
					"\t\t}}"
					, sim.name);
				break;
			case simple_item_type_E::short_E:
				fmt::println(out_file_cpp,
					"\tch_el = find_element(el, \"{0}\");\n"
					"\tif(ch_el != nullptr)\n"
					"\t\t{{\n"
					"\t\tint i;\n"
					"\t\tch_el->QueryIntText(&i);\n"
					"\t\tdata->{0} = (short)i;\n"
					"\t\t}}"
					, sim.name);
				break;
			case simple_item_type_E::unsigned_short_E:
				fmt::println(out_file_cpp,
					"\tch_el = find_element(el, \"{0}\");\n"
					"\tif(ch_el != nullptr)\n"
					"\t\t{{\n"
					"\t\tunsigned int ui;\n"
					"\t\tch_el->QueryUnsignedText(&ui);\n"
					"\t\tdata->{0} = (unsigned short)ui;\n"
					"\t\t}}"
					, sim.name);
				break;
			case simple_item_type_E::long_E:
				fmt::println(out_file_cpp,
					"\tch_el = find_element(el, \"{0}\");\n"
					"\tif(ch_el != nullptr)\n"
					"\t\t{{\n"
					"\t\tint i;\n"
					"\t\tch_el->QueryIntText(&i);\n"
					"\t\tdata->{0} = (long)i;\n"
					"\t\t}}"
					, sim.name);
				break;
			case simple_item_type_E::unsigned_long_E:
				fmt::println(out_file_cpp,
					"\tch_el = find_element(el, \"{0}\");\n"
					"\tif(ch_el != nullptr)\n"
					"\t\t{{\n"
					"\t\tunsigned int ui;\n"
					"\t\tch_el->QueryUnsignedText(&ui);\n"
					"\t\tdata->{0} = (unsigned long)ui;\n"
					"\t\t}}"
					, sim.name);
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

	for (auto &co : s.complex)
		{
		complex_item_type_E line_type = co.line_type;

		switch (line_type)
			{
			case complex_item_type_E::enum_E:
				fmt::println(out_file_cpp,
					"\tch_el = find_element(el, \"{0}\");\n"
					"\tif(ch_el != nullptr)\n"
					"\t\t{{\n"
					"\t\tconst char *pt = ch_el->GetText();\n"
					"\t\tif(pt != nullptr)\n"
					"\t\t\tstfx::s_to_e(pt, data->{0});\n"
					"\t\t}}", co.name);
				break;
			case complex_item_type_E::struct_E:
				fmt::println(out_file_cpp,
//					"\tch_el = find_element(el, \"{0}\");\n"
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




bool process_items_xml_C::process_struct_writer(struct_S const &s, std::string const class_name, std::FILE *out_file_cpp, std::FILE *out_file_h)
	{
	bool rv{ true };

	fmt::println(out_file_h, "\t\tbool do_wr_{0}(tinyxml2::XMLElement *el, {0} *data);", s.name);

	fmt::println(out_file_cpp,
		"bool {0}::do_wr_{1}(tinyxml2::XMLElement *el, {1} *data)\n"
		"\t{{\n"
		"\ttinyxml2::XMLElement *ch_el;\n"
		, class_name, s.name);
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
			case simple_item_type_E::long_E:
			case simple_item_type_E::short_E:
			case simple_item_type_E::unsigned_short_E:
				fmt::println(out_file_cpp,
					"\tif(m_delta_only == false || data->{0} != default_data.{0})\n"
					"\t\t{{\n"
					"\t\tch_el = el->InsertNewChildElement(\"{0}\");\n"
					"\t\tch_el->SetText(data->{0});\n"
					"\t\t}}"
					, sim.name);
				break;
			case simple_item_type_E::std_string_E:
				fmt::println(out_file_cpp,
					"\tif(m_delta_only == false || data->{0} != default_data.{0})\n"
					"\t\t{{\n"
					"\t\tch_el = el->InsertNewChildElement(\"{0}\");\n"
					"\t\tch_el->SetText(data->{0}.c_str());\n"
					"\t\t}}"
					, sim.name);
				break;
			case simple_item_type_E::unsigned_long_E:
				fmt::println(out_file_cpp,
					"\tif(m_delta_only == false || data->{0} != default_data.{0})\n"
					"\t\t{{\n"
					"\t\tch_el = el->InsertNewChildElement(\"{0}\");\n"
					"\t\tch_el->SetText((unsigned)data->{0});\n"
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
#ifdef NOT_NOW
	if (has_children == true)
		{
		fmt::println(out_file_cpp, "\ttinyxml2::XMLElement *ch_el;");
		}
#endif
	for (auto &co : s.complex)
		{
		complex_item_type_E line_type = co.line_type;

		switch (line_type)
			{
			case complex_item_type_E::enum_E:
				fmt::println(out_file_cpp,
					"\tif(m_delta_only == false || data->{0} != default_data.{0})\n"
					"\t\t{{\n"
					"\t\tch_el = el->InsertNewChildElement(\"{0}\");\n"
					"\t\tch_el->SetText(stfx::s_from_e(data->{0}).c_str());\n"
					"\t\t}}"
					, co.name);
				break;
			case complex_item_type_E::struct_E:
				fmt::println(out_file_cpp,
					"\tch_el = el->InsertNewChildElement(\"{0}\");\n"
					"\tif (ch_el != nullptr)\n"
					"\t\t{{\n"
					"\t\tdo_wr_{1}(ch_el, &data->{0});\n"
					"\t\t}}"
					, co.name, co.type_name);
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
					"\t\t}}"
					, co.name, co.type_name);
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
