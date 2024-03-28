// structs.h
// created 2024-01-29 06:19:29.4073232

#pragma once

#include <string>
#include <tinyxml2.h>
#include "config_data.h"

class xml_reader
	{
	public:
		xml_reader();
		virtual bool read_from_file(std::string const &filename, config &struct_to_fill);
	protected:
		bool do_common_input(tinyxml2::XMLElement *el, common_input *data);
		bool do_config(tinyxml2::XMLElement *el, config *data);
		bool do_in_out_spec(tinyxml2::XMLElement *el, in_out_spec *data);
		bool do_output_spec(tinyxml2::XMLElement *el, output_spec *data);

	};
class xml_writer : xml_reader
	{
	private:
		bool m_delta_only {};
	public:
		xml_writer(bool delta_only = true);
		virtual bool write_to_file(std::string const &filename, config &struct_to_read);
	protected:
		bool do_wr_common_input(tinyxml2::XMLElement *el, common_input *data);
		bool do_wr_config(tinyxml2::XMLElement *el, config *data);
		bool do_wr_in_out_spec(tinyxml2::XMLElement *el, in_out_spec *data);
		bool do_wr_output_spec(tinyxml2::XMLElement *el, output_spec *data);

	};
