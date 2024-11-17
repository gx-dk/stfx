// structs.h
// created 2024-10-04 15:39:18.3911740
// Automatically generated using stfx. Do not directly edit this file, use stfx to re-create this file.
// Licence : MIT License

#pragma once

#include <string>
#include <tinyxml2.h>

#include "config_data.h"

class xml_reader_C
	{
	public:
		xml_reader_C();
		virtual bool read_from_file(std::string const &filename, config &struct_to_fill);
	protected:
		bool do_config(tinyxml2::XMLElement *el, config *data);
		bool do_input_file(tinyxml2::XMLElement *el, input_file *data);
		bool do_input_spec(tinyxml2::XMLElement *el, input_spec *data);
		bool do_output_spec(tinyxml2::XMLElement *el, output_spec *data);
		bool do_uncommon_spec(tinyxml2::XMLElement *el, uncommon_spec *data);
	};

class xml_writer_C : public xml_reader_C
	{
	private:
		bool m_delta_only {};
	public:
		xml_writer_C(bool delta_only = true);
		virtual bool write_to_file(std::string const &filename, config &struct_to_read);
	protected:
		bool do_wr_config(tinyxml2::XMLElement *el, config *data);
		bool do_wr_input_file(tinyxml2::XMLElement *el, input_file *data);
		bool do_wr_input_spec(tinyxml2::XMLElement *el, input_spec *data);
		bool do_wr_output_spec(tinyxml2::XMLElement *el, output_spec *data);
		bool do_wr_uncommon_spec(tinyxml2::XMLElement *el, uncommon_spec *data);
	};
