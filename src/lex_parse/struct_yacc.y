/* struct_yacc.y  : bison code to parse C++ enum (class) and structs ONLY   */


%{

#include <vector>
#include <string>
#include <iostream>

#include "items.h"

%}

%require "3.7.4"
%language "C++"
%defines "Parser.hpp"
%output "Parser.cpp"

%define api.value.type variant
%define api.token.raw
%define api.parser.class { Parser }
%define parse.error detailed
%locations
%define api.location.file "Location.hpp"
%parse-param { Lexer *lexer } { info_items_C *info_items } { std::ostream *err }

%token BLOCK_START "{"
%token BLOCK_END "}"
%token EQUALS "="
%token COMMA ","
%token SEMICOLON ";"
%token LESSTHAN "<"
%token GREATERTHAN ">"
%token ENUM "enum"
%token CLASS "class"
%token STRUCT "struct"
%token INT "int"
%token SHORT "short"
%token LONG "long"
%token UNSIGNED "unsigned"
%token FLOAT "float"
%token DOUBLE "double"
%token BOOL "bool"
%token TRUE "true"
%token FALSE "false"
%token STD_STRING "std::string"
%token STD_VECTOR "std::vector"

%token <std::string> NAME
%token <std::string> ENUM_VALUE
%token <std::string> VALUE
%token <std::string> HEXVALUE
%token <std::string> FVALUE
%token <std::string> STRING_CONSTANT
%token <std::string> CHAR_CONSTANT

%nterm <std::string> name
%nterm <enum class simple_item_type_E> simple_type
%nterm <std::string> simple_default_value value
%nterm <std::string> struct_block_vector


%code requires {
		namespace yy {
				class Lexer;
		}
}

%code {
		#include <FlexLexer.h>
		#include "token_if.h"
		#define yylex(a, b) lexer->lex(a, b)
		#define VALID_OP(op1, op2, op, l) \
				if (op1->type() != op2->type()) { \
						error(l, "mixed-mode operations are not allowed"); \
						YYERROR; \
				} \
				else if (valid_ops.at(op).second.find(op1->type()) == valid_ops.at(op).second.end()) { \
						error(l, "operator not supported for this type"); \
						YYERROR; \
				}
}


%%

file_content : parts

parts : part
		| part parts

part : enum_part
	| struct_part


block_start: BLOCK_START

block_end : BLOCK_END

comma : COMMA

semicolon : SEMICOLON

equals : EQUALS

lessthan : LESSTHAN

greaterthan : GREATERTHAN

true : TRUE

false : FALSE

unsigned : UNSIGNED

int : INT

short : SHORT

long : LONG


std_string : STD_STRING

std_vector : STD_VECTOR

enum : ENUM

class : CLASS

struct: STRUCT

float: FLOAT

double: DOUBLE

bool: BOOL



name: NAME														{ $$ = $1; }

enum_part :  enum_start enum_block

enum_start : enum NAME                                          { info_items->process_enum($2, false); }
			| enum class name                                   { info_items->process_enum($3, true); }

enum_block : block_start enum_block_ents block_end semicolon

enum_block_ents : 	enum_ent
			| enum_ent comma enum_block_ents
			| enum_ent comma


enum_ent : name 							                    { info_items->process_enum_line($1, false, "-");}
			|  name equals VALUE                                { info_items->process_enum_line($1, true, $3);}
			|  name equals HEXVALUE                             { info_items->process_enum_line($1, true, $3);}
			|  name equals CHAR_CONSTANT                        { info_items->process_enum_line($1, true, $3);}

struct_part : struct_start block_start struct_block_ents block_end semicolon

struct_start : struct name                                      { info_items->process_struct($2); }

struct_block_ents : struct_block_ent
			| struct_block_ent struct_block_ents

struct_block_ent : simple_type name semicolon                   { info_items->process_struct_line_simple($1, $2, "-");}
			| simple_type name simple_default_value semicolon   { info_items->process_struct_line_simple($1, $2, $3);}
			| name name semicolon                               { info_items->process_struct_line_complex(complex_item_type_E::enum_or_struct_E, $1, $2); }
			| name name simple_default_value semicolon          { info_items->process_struct_line_complex(complex_item_type_E::enum_or_struct_E, $1, $2); }
			| struct_block_vector name semicolon                { info_items->process_struct_line_complex(complex_item_type_E::vector_E, $1, $2); }

simple_type : bool												{ $$ = simple_item_type_E::bool_E; }
			| int												{ $$ = simple_item_type_E::int_E; }
			| short											    { $$ = simple_item_type_E::short_E; }
			| long											    { $$ = simple_item_type_E::long_E; }
			| unsigned int						 				{ $$ = simple_item_type_E::unsigned_int_E; }
			| unsigned short								    { $$ = simple_item_type_E::unsigned_short_E; }
			| unsigned long										{ $$ = simple_item_type_E::unsigned_long_E; }
			| float												{ $$ = simple_item_type_E::float_E; }
			| double										    { $$ = simple_item_type_E::double_E; }
			| std_string									    { $$ = simple_item_type_E::std_string_E; }

simple_default_value : block_start block_end                    { $$ = "{}"; }
			 | block_start value block_end                      { $$ = "{" + $2 + "}"; }

value : VALUE                                                   { $$ = $1; }
	| FVALUE                                                    { $$ = $1; }
	| true                                                      { $$ = "true"; }
	| false                                                     { $$ = "false"; }
	| ENUM_VALUE                                                { $$ = $1; }
	| STRING_CONSTANT                                           { $$ = $1; }
	| CHAR_CONSTANT                                             { $$ = $1; }

struct_block_vector : std_vector lessthan name greaterthan      { $$ = $3; }

%%

void yy::Parser::error(const location_type& loc, const std::string &e) {
		*err << "Location " << loc.begin.line << ':' << loc.begin.column;
		if (loc.end.line != loc.begin.line) {
				*err << '-' << loc.end.line << ':' << loc.end.column - 1;
		}
		else if ((loc.end.column - loc.begin.column) > 1) {
				*err << '-' << loc.end.column - 1;
		}
		*err << ", " << e << '\n';
}

