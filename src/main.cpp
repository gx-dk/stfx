// main.cpp

#include <iostream>		// included ... at least "for now" ... 
#include <fstream>		// etc...
#include <filesystem>
#include <string>
#include <fmt/format.h>

#include "items.h"			// parser.hpp *SHOULD* have included this... but didn't so...

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
#include "process_items.h"
#include "config_data.h"

int process_single_header(std::filesystem::path in_file);
int process_stfx_file(std::filesystem::path in_file);

int main(int argc, char *argv[])
	{
	int rv{};
	if (argc <= 1)
		{
		fmt::print("{} <filename> :\n"
			"\nProgram that creates code that performs the translation of Structs To/From Xml file\n\n"
			"<filename> is <inputheader> headerfile or <setup>.stfx stfx configuration file.\n"
			"<filename> parameter must be given\n", argv[0]);
		exit(1);
		}
	std::filesystem::path in_path(argv[1]);
	if (std::filesystem::exists(in_path) == false)
		{
		fmt::print("{} <filename> :\n"
			"\nProgram that creates code that performs the translation of Structs To/From Xml file\n\n"
			"<filename> is <inputheader> headerfile or <setup>.stfx stfx configuration file.\n"
			"<filename> parameter must be given\n\n"
			"<filename> '{}' does NOT exist\n", argv[0], in_path.string());
		exit(1);
		}
	in_path = std::filesystem::canonical(in_path);
	std::filesystem::path ext = in_path.extension();
	if (ext == "stfx")
		{
		rv = process_stfx_file(in_path);
		}
	else
		{
		rv = process_single_header(in_path);
		}
	return rv;
	}

int process_single_header(std::filesystem::path in_path)
	{
	int rv{};

	std::istream *infile{ nullptr };
	infile = new std::ifstream{ in_path };
	if (infile == nullptr || infile->fail() == true)
		{
		fmt::print("Error: could not open file for reading: {}\n", in_path.string());
		return 1;
		}
	fmt::print("File '{}' opened for input\n", in_path.string());
	std::cin.rdbuf(infile->rdbuf());

	info_items_C info_items;
	yy::Lexer scanner;
	yy::Parser parser(&scanner, &info_items, &std::cerr);

	rv = parser.parse();

	if (rv == 0)
		{
		bool ok;
		fmt::print("Sucessfully parsed file: {}\n", in_path.string());
		in_out_spec file_specs;
		file_specs.in_file = in_path.string();
		file_specs.out.enum_file = "enums";
		file_specs.out.structs_file = "structs";
		process_items_C process_items;
		ok = process_items.process_items(info_items, file_specs);
		fmt::print("Processing items done. Success = {}", ok);
		}
	else
		{
		fmt::print("Failed to correctly parse '{}' code {}", in_path.string(), rv);
		}
	return rv;
	}

int process_stfx_file(std::filesystem::path in_file)
	{
	int rv{};
	return rv;
	}
