// items.cpp

#include <string>
#include <vector>
#include <map>

#include <fmt/format.h>

#include "items.h"

complex_item_type_E info_items_C::get_item_type(const std::string name)
	{
	complex_item_type_E rv{ complex_item_type_E::unknown_E };
	auto stru = m_structs.find(name);
	if (stru != m_structs.end())
		{
		rv = complex_item_type_E::struct_E;
		}
	else
		{
		auto eno = m_enums.find(name);
		if (eno != m_enums.end())
			rv = complex_item_type_E::enum_E;
		}
	return rv;
	}

info_items_C::info_items_C()
	{
	}

bool info_items_C::process_enum(std::string name, bool is_class_enum)
	{
	// fmt::print("process_enum( {}, {} )\n", name, is_class_enum);
	bool rv{ false };
	if (get_item_type(name) != complex_item_type_E::unknown_E)
		{
		fmt::print("ERROR process_enum : name '{} already in use", name);
		}
	else
		{
		enum_S e;
		e.name = name;
		e.is_class_enum = is_class_enum;
		m_enums[name] = e;
		m_current_enum = &m_enums.at(name);
		m_current_struct = nullptr;			// just "in case" !!!
		rv = true;
		}
	return rv;
	}

bool info_items_C::process_enum_line(std::string name, bool initialize_value, std::string value)
	{
	// fmt::print("process_enum_line( {}, {}, {} )\n", name, initialize_value, value);
	bool rv{ false };
	if (m_current_enum != nullptr)
		{
		enum_line_S el;
		el.name = name;
		el.initialize_value = initialize_value;
		el.value = value;
		m_current_enum->enums.push_back(el);
		rv = true;
		}
	return rv;
	}

bool info_items_C::process_struct(std::string name)
	{
	// fmt::print("process_struct_reader( {} )\n", name);
	bool rv{ false };
	if (get_item_type(name) != complex_item_type_E::unknown_E)
		{
		fmt::print("ERROR process_struct : name '{} already in use", name);
		}
	else
		{
		struct_S s;
		s.name = name;
		m_structs[name] = s;
		m_current_struct = &m_structs.at(name);
		m_current_enum = nullptr;			// just "in case" !!!
		rv = true;
		}
	return rv;
	}

bool info_items_C::process_struct_line_simple(simple_item_type_E line_type, std::string name, std::string default_value)
	{
	// fmt::print("process_struct_line_simple( {}, {}, {} )\n", (int)line_type, name, default_value);
	bool rv{ false };
	if (m_current_struct != nullptr)
		{
		struct_line_simple_S s;
		s.line_type = line_type;
		s.name = name;
		s.default_value = default_value;
		m_current_struct->simple.push_back(s);
		rv = true;
		}
	return rv;
	}

bool info_items_C::process_struct_line_complex(complex_item_type_E line_type, std::string type_name, std::string name)
	{
	// fmt::print("process_struct_line_complex( {}, {}, {} )\n", (int)line_type, type_name, name);
	bool rv{ false };
	if (m_current_struct != nullptr)
		{
		struct_line_complex_S s;
		s.line_type = line_type;
		s.type_name = type_name;
		s.name = name;
		m_current_struct->complex.push_back(s);
		rv = true;
		}
	return rv;
	}
