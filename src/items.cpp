// items.cpp

#include "items.h"

#include <filesystem>
#include <fstream>		// etc...
#include <map>
#include <string>
#include <vector>

#include <fmt/format.h>

#ifdef WIN32
#pragma warning( push )
#pragma warning( disable : 4065 )		// we don't want to see warning C4065 from lex/bison generated code... so, we turn it off !!
#endif

#include "parser.hpp"

#ifdef WIN32
#pragma warning( pop )
#endif

#include <FlexLexer.h>
#include "token_if.h"



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

// ===========================================================================

bool info_items_C::process_input_file(const std::filesystem::path& in_path)
{
	bool rv{};

	std::istream* infile{ nullptr };
	infile = new std::ifstream{ in_path };
	if (infile == nullptr || infile->fail() == true)
	{
		fmt::println("ERROR: could not open file for reading: {}\n", in_path.string());
		return false;
	}
	fmt::println("\nFile opened for input:\t\t{}", in_path.string());
	std::cin.rdbuf(infile->rdbuf());

	yy::Lexer scanner;
	yy::Parser parser(&scanner, this, &std::cerr);

	m_parse_rv = parser.parse();
	rv = (m_parse_rv == 0);
	return rv;
}

// ---- and functions called from parser --- 

bool info_items_C::process_enum(std::string name, bool is_class_enum, std::string doc_comment)
	{
	// fmt::print("process_enum( {}, {} )\n", name, is_class_enum);
	bool rv{ false };
	if (get_item_type(name) != complex_item_type_E::unknown_E)
		{
		fmt::println("ERROR process_enum : name '{} already in use", name);
		}
	else
		{
		enum_S e;
		e.name = name;
		e.is_class_enum = is_class_enum;
		e.doc_comment = doc_comment;
		m_enums[name] = e;
		m_current_enum = &m_enums.at(name);
		m_current_struct = nullptr;			// just "in case" !!!
		rv = true;
		}
	return rv;
	}

#ifdef NOT_NOW
bool info_items_C::process_enum_line(std::string name, bool initialize_value, std::string value, std::string doc_comment)
	{
	// fmt::print("process_enum_line( {}, {}, {} )\n", name, initialize_value, value);
	bool rv{ false };
	if (m_current_enum != nullptr)
		{
		enum_line_S el;
		el.name = name;
		el.initialize_value = initialize_value;
		el.value = value;
		el.doc_comment = doc_comment;
		m_current_enum->enums.push_back(el);
		rv = true;
		}
	return rv;
	}
#endif

bool info_items_C::process_enum_line(std::shared_ptr<enum_line_S> e)
	{
	m_current_enum->enums.push_back(*e.get());
	return true;
	}

bool info_items_C::process_struct(std::string name, std::string doc_comment)
	{
	// fmt::print("process_struct_reader( {} )\n", name);
	bool rv{ false };
	if (get_item_type(name) != complex_item_type_E::unknown_E)
		{
		fmt::println("ERROR process_struct : name '{} already in use", name);
		}
	else
		{
		struct_S s;
		s.name = name;
		s.doc_comment = doc_comment;
		m_structs[name] = s;
		m_current_struct = &m_structs.at(name);
		m_current_enum = nullptr;			// just "in case" !!!
		rv = true;
		}
	return rv;
	}

bool info_items_C::process_struct_line_simple(simple_item_type_E line_type, std::string name, std::string default_value, std::string doc_comment)
	{
	// fmt::print("process_struct_line_simple( {}, {}, {} )\n", (int)line_type, name, default_value);
	bool rv{ false };
	if (m_current_struct != nullptr)
		{
		struct_line_simple_S s;
		s.line_type = line_type;
		s.name = name;
		s.default_value = default_value;
		s.doc_comment = doc_comment;
		m_current_struct->simple.push_back(s);
		rv = true;
		}
	return rv;
	}

bool info_items_C::process_struct_line_complex(complex_item_type_E line_type, std::string type_name, std::string name, std::string doc_comment)
	{
	// fmt::print("process_struct_line_complex( {}, {}, {} )\n", (int)line_type, type_name, name);
	bool rv{ false };
	if (m_current_struct != nullptr)
		{
		struct_line_complex_S s;
		s.line_type = line_type;
		s.type_name = type_name;
		s.name = name;
		s.doc_comment = doc_comment;
		m_current_struct->complex.push_back(s);
		rv = true;
		}
	return rv;
	}
