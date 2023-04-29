// structs.cpp
// created 2023-04-29 14:49:23.3733881

#include "structs.h"

#include <string>
#include <map>
#include <stdexcept>

#include <tinyxml2.h>
#include <tixml2ex.h>

#include "enums.h"
#include "config_data.h"


xml_reader::xml_reader()
	{
	}


bool xml_reader::read_from_file(std::string const &filename, config &struct_to_fill)
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

bool xml_reader::do_common_input(tinyxml2::XMLElement *el, common_input *data)
	{
	bool rv = true;
		{
		const char *pt;
		if (tinyxml2::XML_SUCCESS == el->QueryAttribute("in_file", &pt))
			data->in_file = pt;
		}
	return rv;
	};

bool xml_reader::do_config(tinyxml2::XMLElement *el, config *data)
	{
	bool rv = true;
	tinyxml2::XMLElement *ch_el;
	ch_el = el->FirstChildElement("common_in_files");
	if (ch_el != nullptr)
		{
		for (auto item : ch_el)
			{
			common_input s;
			if (do_common_input(item, &s) == true)
				{
				data->common_in_files.push_back(s);
				}
			}
		}
	ch_el = el->FirstChildElement("common_out_files");
	if (ch_el != nullptr)
		{
		do_output_spec(ch_el, &data->common_out_files);
		}
	ch_el = el->FirstChildElement("in_out_files");
	if (ch_el != nullptr)
		{
		for (auto item : ch_el)
			{
			in_out_spec s;
			if (do_in_out_spec(item, &s) == true)
				{
				data->in_out_files.push_back(s);
				}
			}
		}
	return rv;
	};

bool xml_reader::do_in_out_spec(tinyxml2::XMLElement *el, in_out_spec *data)
	{
	bool rv = true;
		{
		const char *pt;
		if (tinyxml2::XML_SUCCESS == el->QueryAttribute("in_file", &pt))
			data->in_file = pt;
		}
	tinyxml2::XMLElement *ch_el;
	ch_el = el->FirstChildElement("out");
	if (ch_el != nullptr)
		{
		do_output_spec(ch_el, &data->out);
		}
	return rv;
	};

bool xml_reader::do_output_spec(tinyxml2::XMLElement *el, output_spec *data)
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
	return rv;
	};




xml_writer::xml_writer(bool delta_only) : xml_reader()
{
	m_delta_only = delta_only;
}


bool xml_writer::write_to_file(std::string const &filename, config &struct_to_read)
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

bool xml_writer::do_wr_common_input(tinyxml2::XMLElement *el, common_input *data)
	{
	bool rv = true;
	common_input default_data;
	if(m_delta_only == false || data->in_file != default_data.in_file)
		el->SetAttribute("in_file", data->in_file.c_str());
	return rv;
	};

bool xml_writer::do_wr_config(tinyxml2::XMLElement *el, config *data)
	{
	bool rv = true;
	config default_data;
	tinyxml2::XMLElement *ch_el;
	ch_el = el->InsertNewChildElement("common_in_files");
	if (ch_el != nullptr)
		{
		for (common_input itr : data->common_in_files)
			{
			tinyxml2::XMLElement *ch_ch_el = ch_el->InsertNewChildElement("common_input");
			do_wr_common_input(ch_ch_el, &itr);
			}
		}
	ch_el = el->InsertNewChildElement("common_out_files");
	if (ch_el != nullptr)
		{
		do_wr_output_spec(ch_el, &data->common_out_files);
		}
	ch_el = el->InsertNewChildElement("in_out_files");
	if (ch_el != nullptr)
		{
		for (in_out_spec itr : data->in_out_files)
			{
			tinyxml2::XMLElement *ch_ch_el = ch_el->InsertNewChildElement("in_out_spec");
			do_wr_in_out_spec(ch_ch_el, &itr);
			}
		}
	return rv;
	};

bool xml_writer::do_wr_in_out_spec(tinyxml2::XMLElement *el, in_out_spec *data)
	{
	bool rv = true;
	in_out_spec default_data;
	if(m_delta_only == false || data->in_file != default_data.in_file)
		el->SetAttribute("in_file", data->in_file.c_str());
	tinyxml2::XMLElement *ch_el;
	ch_el = el->InsertNewChildElement("out");
	if (ch_el != nullptr)
		{
		do_wr_output_spec(ch_el, &data->out);
		}
	return rv;
	};

bool xml_writer::do_wr_output_spec(tinyxml2::XMLElement *el, output_spec *data)
	{
	bool rv = true;
	output_spec default_data;
	if(m_delta_only == false || data->relative_directory != default_data.relative_directory)
		el->SetAttribute("relative_directory", data->relative_directory.c_str());
	if(m_delta_only == false || data->enum_file != default_data.enum_file)
		el->SetAttribute("enum_file", data->enum_file.c_str());
	if(m_delta_only == false || data->structs_file != default_data.structs_file)
		el->SetAttribute("structs_file", data->structs_file.c_str());
	return rv;
	};



