// process_items.h

#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "config_data.h"
#include "items.h"


class process_items_C
	{
	protected:
		std::filesystem::path m_base_dir_path;
		std::string m_stfx_info;

	public:
		process_items_C(std::filesystem::path base_dir_path);;

		bool process_items(info_items_C &items, const std::vector<std::string> &input_files, const output_spec &output);

	protected:
		bool process_all_enums(info_items_C &items, const std::vector<std::string> &input_files, const output_spec &output);
		bool process_enum(const enum_S &e, std::FILE *out_file_cpp, std::FILE *out_file_h);
		bool create_fmt_templates_for_enum(const enum_S &e, std::FILE *out_file_h);

		bool fixup_types_of_names(info_items_C &items);
		bool find_top_struct(info_items_C &items);

		// the following functions should be overriden in derived classes... 

		virtual bool process_all_structs(info_items_C &items, const std::vector<std::string> &input_files, const output_spec &output) = 0;
		virtual bool process_struct_reader(struct_S const &s, std::string const class_name, std::FILE *out_file_cpp, std::FILE *out_file_h) = 0;
		virtual bool process_struct_writer(struct_S const &s, std::string const class_name, std::FILE *out_file_cpp, std::FILE *out_file_h, bool no_special_delta) = 0;
	};

