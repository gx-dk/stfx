// process_items.h

#pragma once

#include "items.h"
#include "config_data.h"

class process_items_xml_C
	{
	protected:
		std::string m_timestamp;
		std::filesystem::path m_base_dir_path;

	public:
		process_items_xml_C(std::filesystem::path base_dir_path) : m_base_dir_path(base_dir_path)
			{
			};
		bool process_items(info_items_C &items, const std::vector<std::string>&input_files, const output_spec &output);

	protected:
		bool fixup_types_of_names(info_items_C &items);
		bool find_top_struct(info_items_C &items);

		bool process_all_enums(info_items_C &items, const std::vector<std::string> &input_files, const output_spec &output);
		bool process_enum(const enum_S &e, std::FILE *out_file_cpp, std::FILE *out_file_h);
		bool create_fmt_templates_for_enum(const enum_S &e, std::FILE *out_file_h);

		bool process_all_structs(info_items_C &items, const std::vector<std::string> &input_files, const output_spec & output);
		virtual bool process_struct_reader(struct_S const &s, std::string const class_name, std::FILE *out_file_cpp, std::FILE *out_file_h);
		virtual bool process_struct_writer(struct_S const &s, std::string const class_name, std::FILE *out_file_cpp, std::FILE *out_file_h);
	};


class process_items_xml_attrib_C : public process_items_xml_C
	{
	public:
		process_items_xml_attrib_C(std::filesystem::path base_dir_path) : process_items_xml_C(base_dir_path)
		{
		};
	protected:
	bool process_struct_reader(struct_S const &s, std::string const class_name, std::FILE *out_file_cpp, std::FILE *out_file_h) override;
	bool process_struct_writer(struct_S const &s, std::string const class_name, std::FILE *out_file_cpp, std::FILE *out_file_h) override;
	};