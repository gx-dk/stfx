// main.cpp

#include <iostream>		// included ... at least "for now" ... 
#include <fstream>		// etc...
#include <string>
#include <fmt/format.h>

#include "items.h"			// parser.hpp *SHOULD* have included this... but didn't so...
#include "parser.hpp"
#include <FlexLexer.h>
#include "token_if.h"
#include "process_items.h"



int main(int argc, char* argv[])
	{
	int rv {};
	const char* in_file_name = argc > 1 ? argv[1] : "test_input_config_camera.h";

	fmt::print("\nstarting {}. Input from :  {}\n\n", argv[0], in_file_name);

	std::istream* infile{ nullptr };
	infile = new std::ifstream{ in_file_name };
	if (infile == nullptr || infile->fail() == true)
		{
		fmt::print("Error: could not open file for reading: {}\n", in_file_name);
		return 1;
		}
	fmt::print("File {} opened for input\n", in_file_name);
	std::cin.rdbuf(infile->rdbuf());

	info_items_C info_items;
	yy::Lexer scanner ;
	yy::Parser parser( &scanner, &info_items, &std::cerr );

	rv = parser.parse();

	if (rv == 0)
		{
		bool ok;
		fmt::print("Sucessfully parsed file {}\n", in_file_name);
		process_items_C process_items;
		ok = process_items.process_items(info_items, in_file_name);
		fmt::print("Processing items done. Success = {}", ok);
		}
	else
		{
		fmt::print("Failed to correctly parse {} code {}", in_file_name, rv);
		}

	return rv;
 	}





