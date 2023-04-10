/* hello world https://learnmoderncpp.com/2020/12/16/generating-c-programs-with-flex-and-bison-1/  */

%{

#include "items.h"          // parser.hpp *SHOULD* have included this, but didn't :-( ... so needs to go first ..

#include  "parser.hpp"
#include "token_if.h"

#define YY_DECL int yy::Lexer::lex(Parser::semantic_type *value, location *loc)
using namespace yy;

%}
%x C_COMMENT

%option noyywrap c++ outfile="tokenize.cpp"
                /* header="tokenize.hpp" */

DIGIT    [0-9]
HEXDIGIT   [0-9A-Fa-f]
ID	    [a-zA-Z_][a-zA_Z0-9_]*
SEP_CHAR  [ \t]
C_COMMENT_START [/][*]
C_COMMENT_END   [*][/]
CPP_COMMENT_START [/][/]
QUOTE   ["]

%%

"/*"            {
                /* start dealing with C style comments, so switch to state that processes C comments  */
                BEGIN(C_COMMENT);
                }
<C_COMMENT>\n   {
                /* newline in C Style comment ... */
                loc->begin = loc->end; loc->lines(1);
                }
<C_COMMENT>.    {
                /* anything else in C Style comment ... */
                loc->begin = loc->end; loc->columns(YYLeng());
                }
<C_COMMENT>"*/" {
                /* when reached end of C style comment, switch to normal processing state */
                BEGIN(INITIAL); 
                }

{CPP_COMMENT_START}.*$			{
                /* ... should match c++ style comments */
                loc->begin = loc->end; loc->columns(YYLeng());
                // printf("ignoring C++ comment : %s\n", yytext);
                }

^[ \t]*#.*$		{
                /* ... should match # lines ... */
                loc->begin = loc->end; loc->columns(YYLeng());
                printf("WARNING : [%d, %d] ignoring pre-processor line : %s\n", loc->begin.line, loc->begin.column, yytext);
                }



[\r]?\n         {
                /* process end of line sequences */
                loc->begin = loc->end; loc->lines(1);
                /* ... eat whitespace... but don't return it to parser! 
                    return yy::Parser::token::SEPARATOR;
                */
                }

{SEP_CHAR}+     {
                loc->begin = loc->end; loc->columns(YYLeng());
                /* ... eat whitespace... but don't return it to parser! 
                    return yy::Parser::token::SEPARATOR;
                */
                }


"{"				{
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::BLOCK_START;
				}

"}"				{
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::BLOCK_END;
				}

"="				{
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::EQUALS;
				}

","				{
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::COMMA;
				}

";"				{
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::SEMICOLON;
				}

"<"				{
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::LESSTHAN;
				}

">"				{
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::GREATERTHAN;
				}

"enum"          {
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::ENUM;
                }

"class"         {
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::CLASS;
                }

"struct"        {
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::STRUCT;
                }

"int"           {
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::INT;
                }

"short"         {
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::SHORT;
                }

"long"          {
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::LONG;
                }

"unsigned"      {
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::UNSIGNED;
                }

"float"         {
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::FLOAT;
                }

"double"         {
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::DOUBLE;
                }

"bool"          {
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::BOOL;
                }

"true"          {
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::TRUE;
                }

"false"         {
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::FALSE;
                }

"std::string"   {
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::STD_STRING;
                }

"std::vector"   {
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::STD_VECTOR;
                }

{ID}+           {
                value->emplace<std::string>(std::string{ YYText(), YYText() + YYLeng() }); 
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::NAME;
                }

{ID}+::{ID}+    {
                value->emplace<std::string>(std::string{ YYText(), YYText() + YYLeng() }); 
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::ENUM_VALUE;
                }

{DIGIT}+		{
                value->emplace<std::string>(std::string{ YYText(), YYText() + YYLeng() }); 
                loc->begin = loc->end; loc->columns(YYLeng());
				return yy::Parser::token::VALUE;
				}

"0x"{HEXDIGIT}+	{
                value->emplace<std::string>(std::string{ YYText(), YYText() + YYLeng() }); 
                loc->begin = loc->end; loc->columns(YYLeng());
				return yy::Parser::token::HEXVALUE;
				}

{DIGIT}+.{DIGIT}+f? {
                value->emplace<std::string>(std::string{ YYText(), YYText() + YYLeng() }); 
                loc->begin = loc->end; loc->columns(YYLeng());
				return yy::Parser::token::FVALUE;
				}

{QUOTE}.*{QUOTE}    {
                value->emplace<std::string>(std::string{ YYText(), YYText() + YYLeng() }); 
                loc->begin = loc->end; loc->columns(YYLeng());
                return yy::Parser::token::STRING_CONSTANT;
                }

%%

/* we copied this from elsewhere ... we don't want to use this yylex !! */ 
int yyFlexLexer::yylex() { throw std::runtime_error("bad call to yyFlexLexer::yylex()"); }

