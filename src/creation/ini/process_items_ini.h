// process_items.h

#pragma once

#include "items.h"
#include "config_data.h"
#include "process_items.h"

class process_items_ini_C : public process_items_C
	{
	protected:

	public:
		process_items_ini_C(std::filesystem::path base_dir_path) : process_items_C(base_dir_path)
			{
			};

	protected:

		bool process_all_structs(info_items_C &items, const std::vector<std::string> &input_files, const output_spec & output);
		bool process_struct_reader(struct_S const &s, std::string const class_name, std::FILE *out_file_cpp, std::FILE *out_file_h) override;
		bool process_struct_writer(struct_S const &s, std::string const class_name, std::FILE *out_file_cpp, std::FILE *out_file_h) override;
	};


class process_items_n_ini_C : public process_items_ini_C
	{
	public:
		process_items_n_ini_C(std::filesystem::path base_dir_path) : process_items_ini_C(base_dir_path)
		{
		};
	protected:
	bool process_struct_reader(struct_S const &s, std::string const class_name, std::FILE *out_file_cpp, std::FILE *out_file_h) override;
	bool process_struct_writer(struct_S const &s, std::string const class_name, std::FILE *out_file_cpp, std::FILE *out_file_h) override;
	private:
		inline void replace_underline_with_space(std::string &str)
			{
			std::replace(str.begin(), str.end(), '_', ' ');
			}
	};