// process_items.h

#pragma once

#include "items.h"
#include "config_data.h"

class process_items_C
	{
	private:
		std::string m_timestamp;

	public:
		bool process_items(info_items_C &items, in_out_spec const &files_specification);

	private:
		bool fixup_types_of_names(info_items_C &items);
		bool find_top_struct(info_items_C &items);

		bool process_all_enums(info_items_C & items, in_out_spec const &files_specification);
		bool process_enum(const enum_S &e, std::string in_filename, std::FILE * out_file_cpp, std::FILE* out_file_h);
		bool create_fmt_templates_for_enum(const enum_S& e, std::FILE* out_file_h);

		bool process_all_structs(info_items_C & items, in_out_spec const & file_specifications);
		bool process_struct_reader(struct_S const& s, std::string const class_name, std::FILE* out_file_cpp, std::FILE* out_file_h);
		bool process_struct_writer(struct_S const& s, std::string const class_name, std::FILE* out_file_cpp, std::FILE* out_file_h);
	};

