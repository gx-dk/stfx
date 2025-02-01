// structs.cpp
//
// Automatically generated using stfx
// Do not directly edit this file, use stfx to re-create this file.
//
// License : MIT License
//
// stfx built at : Feb  1 2025 04:31:50, Software Version : 25.01

#include "structs.h"

#include <string>
#include <map>
#include <stdexcept>
#include <tinyxml2.h>
#include <tixml2ex.h>

#include "enums.h"
#include "config_data.h"

xml_reader_C::xml_reader_C()
	{
	}

bool xml_reader_C::read_from_file(std::string const &filename, config &struct_to_fill)
	{
	bool rv{};
	tinyxml2::XMLError er;
	tinyxml2::XMLDocument doc;

	er = doc.LoadFile(filename.c_str());
	if (er == tinyxml2::XML_SUCCESS)
		{
		tinyxml2::XMLElement* el = doc.RootElement();
		rv = do_config(el, &struct_to_fill);
		}
	return rv;
	}

bool xml_reader_C::do_config(tinyxml2::XMLElement *el, config *data)
	{
	bool rv = true;
	tinyxml2::XMLElement *ch_el;
	ch_el = el->FirstChildElement("common_in_files");
	if (ch_el != nullptr)
		{
		do_input_spec(ch_el, &data->common_in_files);
		}
	ch_el = el->FirstChildElement("common_out_files");
	if (ch_el != nullptr)
		{
		do_output_spec(ch_el, &data->common_out_files);
		}
	ch_el = el->FirstChildElement("non_common");
	if (ch_el != nullptr)
		{
		for (auto item : ch_el)
			{
			uncommon_spec s;
			if (do_uncommon_spec(item, &s) == true)
				{
				data->non_common.push_back(s);
				}
			}
		}
	return rv;
	};

bool xml_reader_C::do_input_file(tinyxml2::XMLElement *el, input_file *data)
	{
	bool rv = true;
		{
		const char *pt;
		if (tinyxml2::XML_SUCCESS == el->QueryAttribute("name", &pt))
			data->name = pt;
		}
	return rv;
	};

bool xml_reader_C::do_input_spec(tinyxml2::XMLElement *el, input_spec *data)
	{
	bool rv = true;
	tinyxml2::XMLElement *ch_el;
	ch_el = el->FirstChildElement("input");
	if (ch_el != nullptr)
		{
		for (auto item : ch_el)
			{
			input_file s;
			if (do_input_file(item, &s) == true)
				{
				data->input.push_back(s);
				}
			}
		}
	return rv;
	};

bool xml_reader_C::do_output_spec(tinyxml2::XMLElement *el, output_spec *data)
	{
	bool rv = true;
		{
		const char *pt;
		if (tinyxml2::XML_SUCCESS == el->QueryAttribute("relative_directory", &pt))
			data->relative_directory = pt;
		}
		{
		const char *pt;
		if (tinyxml2::XML_SUCCESS == el->QueryAttribute("enum_file", &pt))
			data->enum_file = pt;
		}
		{
		const char *pt;
		if (tinyxml2::XML_SUCCESS == el->QueryAttribute("structs_file", &pt))
			data->structs_file = pt;
		}
		{
		const char *pt;
		if (tinyxml2::XML_SUCCESS == el->QueryAttribute("structs_reader_class", &pt))
			data->structs_reader_class = pt;
		}
		{
		const char *pt;
		if (tinyxml2::XML_SUCCESS == el->QueryAttribute("structs_writer_class", &pt))
			data->structs_writer_class = pt;
		}
	el->QueryAttribute("only_read_code", &data->only_read_code);
	el->QueryAttribute("no_special_delta", &data->no_special_delta);
		{
		const char *pt;
		if (tinyxml2::XML_SUCCESS == el->QueryAttribute("file_type", &pt))
			stfx::s_to_e(pt, data->file_type);
		}
	return rv;
	};

bool xml_reader_C::do_uncommon_spec(tinyxml2::XMLElement *el, uncommon_spec *data)
	{
	bool rv = true;
	tinyxml2::XMLElement *ch_el;
	ch_el = el->FirstChildElement("input");
	if (ch_el != nullptr)
		{
		for (auto item : ch_el)
			{
			input_file s;
			if (do_input_file(item, &s) == true)
				{
				data->input.push_back(s);
				}
			}
		}
	ch_el = el->FirstChildElement("out");
	if (ch_el != nullptr)
		{
		do_output_spec(ch_el, &data->out);
		}
	return rv;
	};




xml_writer_C::xml_writer_C(bool delta_only) : xml_reader_C()
{
	m_delta_only = delta_only;
}

bool xml_writer_C::write_to_file(std::string const &filename, config &struct_to_read)
	{
	bool rv{};
	tinyxml2::XMLError er;
	tinyxml2::XMLDocument doc_w;
	auto ch = doc_w.NewElement("config");
	doc_w.InsertFirstChild(ch);
	tinyxml2::XMLElement * el = doc_w.RootElement();
	do_wr_config(el, &struct_to_read);
	er = doc_w.SaveFile(filename.c_str());
	rv = (er == tinyxml2::XML_SUCCESS);
	return rv;
	}

bool xml_writer_C::do_wr_config(tinyxml2::XMLElement *el, config *data)
	{
	bool rv = true;
	config default_data;
	tinyxml2::XMLElement *ch_el;
	ch_el = el->InsertNewChildElement("common_in_files");
	if (ch_el != nullptr)
		{
		do_wr_input_spec(ch_el, &data->common_in_files);
		}
	ch_el = el->InsertNewChildElement("common_out_files");
	if (ch_el != nullptr)
		{
		do_wr_output_spec(ch_el, &data->common_out_files);
		}
	ch_el = el->InsertNewChildElement("non_common");
	if (ch_el != nullptr)
		{
		for (uncommon_spec itr : data->non_common)
			{
			tinyxml2::XMLElement *ch_ch_el = ch_el->InsertNewChildElement("uncommon_spec");
			do_wr_uncommon_spec(ch_ch_el, &itr);
			}
		}
	return rv;
	};

bool xml_writer_C::do_wr_input_file(tinyxml2::XMLElement *el, input_file *data)
	{
	bool rv = true;
	input_file default_data;
	if(m_delta_only == false || data->name != default_data.name)
		el->SetAttribute("name", data->name.c_str());
	return rv;
	};

bool xml_writer_C::do_wr_input_spec(tinyxml2::XMLElement *el, input_spec *data)
	{
	bool rv = true;
	input_spec default_data;
	tinyxml2::XMLElement *ch_el;
	ch_el = el->InsertNewChildElement("input");
	if (ch_el != nullptr)
		{
		for (input_file itr : data->input)
			{
			tinyxml2::XMLElement *ch_ch_el = ch_el->InsertNewChildElement("input_file");
			do_wr_input_file(ch_ch_el, &itr);
			}
		}
	return rv;
	};

bool xml_writer_C::do_wr_output_spec(tinyxml2::XMLElement *el, output_spec *data)
	{
	bool rv = true;
	output_spec default_data;
	if(m_delta_only == false || data->relative_directory != default_data.relative_directory)
		el->SetAttribute("relative_directory", data->relative_directory.c_str());
	if(m_delta_only == false || data->enum_file != default_data.enum_file)
		el->SetAttribute("enum_file", data->enum_file.c_str());
	if(m_delta_only == false || data->structs_file != default_data.structs_file)
		el->SetAttribute("structs_file", data->structs_file.c_str());
	if(m_delta_only == false || data->structs_reader_class != default_data.structs_reader_class)
		el->SetAttribute("structs_reader_class", data->structs_reader_class.c_str());
	if(m_delta_only == false || data->structs_writer_class != default_data.structs_writer_class)
		el->SetAttribute("structs_writer_class", data->structs_writer_class.c_str());
	if(m_delta_only == false || data->only_read_code != default_data.only_read_code)
		el->SetAttribute("only_read_code", data->only_read_code);
	if(m_delta_only == false || data->no_special_delta != default_data.no_special_delta)
		el->SetAttribute("no_special_delta", data->no_special_delta);
	if(m_delta_only == false || data->file_type != default_data.file_type)
		el->SetAttribute("file_type", stfx::s_from_e(data->file_type).c_str());
	return rv;
	};

bool xml_writer_C::do_wr_uncommon_spec(tinyxml2::XMLElement *el, uncommon_spec *data)
	{
	bool rv = true;
	uncommon_spec default_data;
	tinyxml2::XMLElement *ch_el;
	ch_el = el->InsertNewChildElement("input");
	if (ch_el != nullptr)
		{
		for (input_file itr : data->input)
			{
			tinyxml2::XMLElement *ch_ch_el = ch_el->InsertNewChildElement("input_file");
			do_wr_input_file(ch_ch_el, &itr);
			}
		}
	ch_el = el->InsertNewChildElement("out");
	if (ch_el != nullptr)
		{
		do_wr_output_spec(ch_el, &data->out);
		}
	return rv;
	};



