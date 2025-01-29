// items.h

#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <vector>

struct enum_line_S
	{
	std::string name;
	bool initialize_value;
	std::string value;
	std::string doc_comment;
	};

struct enum_S
	{
	std::string name;
	std::string doc_comment;
	bool is_class_enum;
	std::vector<enum_line_S> enums;
	};

enum class simple_item_type_E
	{
	bool_E,
	int_E,
	short_E,
	long_E,
	unsigned_int_E,
	unsigned_short_E,
	unsigned_long_E,
	float_E,
	double_E,
	std_string_E
	};

struct struct_line_simple_S
	{
	simple_item_type_E line_type;
	std::string name;
	std::string default_value;
	std::string doc_comment;
	};

enum class complex_item_type_E
	{
	unknown_E,				/* not yet known ... */
	enum_or_struct_E,		/* when we are unsure ... !!! */
	enum_E,
	struct_E,
	vector_E
	};


struct struct_line_complex_S
	{
	complex_item_type_E line_type;
	std::string type_name;
	std::string name;
	std::string doc_comment;
	};

struct struct_S
	{
	std::string name;
	std::string doc_comment;
	std::vector<struct_line_simple_S> simple;
	std::vector<struct_line_complex_S> complex;
	int incoming_count{ 0 };
	};

struct name_table_enum_items_S
	{
	std::string name;
	enum_S enum_data;
	};



struct name_table_struct_items_S
	{
	std::string name;
	struct_S struct_data;
	};


namespace yy
{
	class Parser;
}

class info_items_C
	{
	private:
		std::map< std::string, struct_S > m_structs;
		std::map< std::string, enum_S > m_enums;
		struct_S *m_current_struct{ nullptr };
		enum_S *m_current_enum{ nullptr };
		int m_parse_rv{};

	public:

		bool process_input_file(const std::filesystem::path& in_path);
		int get_parse_rv()
		{
			return m_parse_rv;
		}

		complex_item_type_E get_item_type(const std::string name);

		const std::map< std::string, enum_S >& get_enums()
		{
			return m_enums;
		};

		std::map< std::string, struct_S >& get_structs()
		{
			return m_structs;
		};

	protected:
		friend yy::Parser;

		bool process_enum(std::string name, bool is_class_enum, std::string doc_comment);
		bool process_enum(std::string name, bool is_class_enum)
			{
			return process_enum(name, is_class_enum, "");
			};
		std::shared_ptr<enum_line_S> make_enum_line(std::string name, bool initialize_value, std::string value)
			{
			std::shared_ptr<enum_line_S> e = std::make_shared<enum_line_S>();
			e->name = name;
			e->initialize_value = initialize_value;
			e->value = value;
			e->doc_comment = "";
			return e;
			};
#ifdef NOT_NOW
		bool process_enum_line(std::string name, bool initialize_value, std::string value, std::string doc_comment);
		bool process_enum_line(std::string name, bool initialize_value, std::string value)
			{
			return process_enum_line(name, initialize_value, value, "");
			};
#endif
		bool process_enum_line(std::shared_ptr<enum_line_S> e);
		bool process_struct(std::string name, std::string doc_comment);
		bool process_struct(std::string name)
			{
			return process_struct(name, "");
			};
		bool process_struct_line_simple(simple_item_type_E line_type, std::string name, std::string default_value, std::string doc_comment);
		bool process_struct_line_simple(simple_item_type_E line_type, std::string name, std::string default_value)
			{
			return process_struct_line_simple(line_type, name, default_value, "");
			}
		bool process_struct_line_complex(complex_item_type_E line_type, std::string type_name, std::string name, std::string doc_comment);
		bool process_struct_line_complex(complex_item_type_E line_type, std::string type_name, std::string name)
			{
			return process_struct_line_complex(line_type, type_name, name, "");
			}
	};

