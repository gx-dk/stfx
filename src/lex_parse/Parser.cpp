// A Bison parser, made by GNU Bison 3.7.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.



// First part of user prologue.
#line 4 "struct_yacc.y"


#include <vector>
#include <string>
#include <iostream>

#include "items.h"


#line 51 "Parser.cpp"


#include "Parser.hpp"


// Unqualified %code blocks.
#line 70 "struct_yacc.y"

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

#line 73 "Parser.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 165 "Parser.cpp"

  /// Build a parser object.
   Parser :: Parser  (Lexer *lexer_yyarg, info_items_C *info_items_yyarg, std::ostream *err_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      lexer (lexer_yyarg),
      info_items (info_items_yyarg),
      err (err_yyarg)
  {}

   Parser ::~ Parser  ()
  {}

   Parser ::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | symbol kinds.  |
  `---------------*/

  // basic_symbol.
  template <typename Base>
   Parser ::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_simple_type: // simple_type
        value.copy< enum class simple_item_type_E > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NAME: // NAME
      case symbol_kind::S_ENUM_VALUE: // ENUM_VALUE
      case symbol_kind::S_VALUE: // VALUE
      case symbol_kind::S_HEXVALUE: // HEXVALUE
      case symbol_kind::S_FVALUE: // FVALUE
      case symbol_kind::S_STRING_CONSTANT: // STRING_CONSTANT
      case symbol_kind::S_CHAR_CONSTANT: // CHAR_CONSTANT
      case symbol_kind::S_DOC_COMMENT: // DOC_COMMENT
      case symbol_kind::S_name: // name
      case symbol_kind::S_simple_default_value: // simple_default_value
      case symbol_kind::S_value: // value
      case symbol_kind::S_struct_block_vector: // struct_block_vector
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }



  template <typename Base>
   Parser ::symbol_kind_type
   Parser ::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }

  template <typename Base>
  bool
   Parser ::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
   Parser ::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_simple_type: // simple_type
        value.move< enum class simple_item_type_E > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_NAME: // NAME
      case symbol_kind::S_ENUM_VALUE: // ENUM_VALUE
      case symbol_kind::S_VALUE: // VALUE
      case symbol_kind::S_HEXVALUE: // HEXVALUE
      case symbol_kind::S_FVALUE: // FVALUE
      case symbol_kind::S_STRING_CONSTANT: // STRING_CONSTANT
      case symbol_kind::S_CHAR_CONSTANT: // CHAR_CONSTANT
      case symbol_kind::S_DOC_COMMENT: // DOC_COMMENT
      case symbol_kind::S_name: // name
      case symbol_kind::S_simple_default_value: // simple_default_value
      case symbol_kind::S_value: // value
      case symbol_kind::S_struct_block_vector: // struct_block_vector
        value.move< std::string > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
   Parser ::by_kind::by_kind ()
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
   Parser ::by_kind::by_kind (by_kind&& that)
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

   Parser ::by_kind::by_kind (const by_kind& that)
    : kind_ (that.kind_)
  {}

   Parser ::by_kind::by_kind (token_kind_type t)
    : kind_ (yytranslate_ (t))
  {}

  void
   Parser ::by_kind::clear ()
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
   Parser ::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

   Parser ::symbol_kind_type
   Parser ::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }

   Parser ::symbol_kind_type
   Parser ::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  // by_state.
   Parser ::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

   Parser ::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
   Parser ::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
   Parser ::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

   Parser ::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

   Parser ::symbol_kind_type
   Parser ::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

   Parser ::stack_symbol_type::stack_symbol_type ()
  {}

   Parser ::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_simple_type: // simple_type
        value.YY_MOVE_OR_COPY< enum class simple_item_type_E > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NAME: // NAME
      case symbol_kind::S_ENUM_VALUE: // ENUM_VALUE
      case symbol_kind::S_VALUE: // VALUE
      case symbol_kind::S_HEXVALUE: // HEXVALUE
      case symbol_kind::S_FVALUE: // FVALUE
      case symbol_kind::S_STRING_CONSTANT: // STRING_CONSTANT
      case symbol_kind::S_CHAR_CONSTANT: // CHAR_CONSTANT
      case symbol_kind::S_DOC_COMMENT: // DOC_COMMENT
      case symbol_kind::S_name: // name
      case symbol_kind::S_simple_default_value: // simple_default_value
      case symbol_kind::S_value: // value
      case symbol_kind::S_struct_block_vector: // struct_block_vector
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

   Parser ::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_simple_type: // simple_type
        value.move< enum class simple_item_type_E > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NAME: // NAME
      case symbol_kind::S_ENUM_VALUE: // ENUM_VALUE
      case symbol_kind::S_VALUE: // VALUE
      case symbol_kind::S_HEXVALUE: // HEXVALUE
      case symbol_kind::S_FVALUE: // FVALUE
      case symbol_kind::S_STRING_CONSTANT: // STRING_CONSTANT
      case symbol_kind::S_CHAR_CONSTANT: // CHAR_CONSTANT
      case symbol_kind::S_DOC_COMMENT: // DOC_COMMENT
      case symbol_kind::S_name: // name
      case symbol_kind::S_simple_default_value: // simple_default_value
      case symbol_kind::S_value: // value
      case symbol_kind::S_struct_block_vector: // struct_block_vector
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
   Parser ::stack_symbol_type&
   Parser ::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_simple_type: // simple_type
        value.copy< enum class simple_item_type_E > (that.value);
        break;

      case symbol_kind::S_NAME: // NAME
      case symbol_kind::S_ENUM_VALUE: // ENUM_VALUE
      case symbol_kind::S_VALUE: // VALUE
      case symbol_kind::S_HEXVALUE: // HEXVALUE
      case symbol_kind::S_FVALUE: // FVALUE
      case symbol_kind::S_STRING_CONSTANT: // STRING_CONSTANT
      case symbol_kind::S_CHAR_CONSTANT: // CHAR_CONSTANT
      case symbol_kind::S_DOC_COMMENT: // DOC_COMMENT
      case symbol_kind::S_name: // name
      case symbol_kind::S_simple_default_value: // simple_default_value
      case symbol_kind::S_value: // value
      case symbol_kind::S_struct_block_vector: // struct_block_vector
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

   Parser ::stack_symbol_type&
   Parser ::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_simple_type: // simple_type
        value.move< enum class simple_item_type_E > (that.value);
        break;

      case symbol_kind::S_NAME: // NAME
      case symbol_kind::S_ENUM_VALUE: // ENUM_VALUE
      case symbol_kind::S_VALUE: // VALUE
      case symbol_kind::S_HEXVALUE: // HEXVALUE
      case symbol_kind::S_FVALUE: // FVALUE
      case symbol_kind::S_STRING_CONSTANT: // STRING_CONSTANT
      case symbol_kind::S_CHAR_CONSTANT: // CHAR_CONSTANT
      case symbol_kind::S_DOC_COMMENT: // DOC_COMMENT
      case symbol_kind::S_name: // name
      case symbol_kind::S_simple_default_value: // simple_default_value
      case symbol_kind::S_value: // value
      case symbol_kind::S_struct_block_vector: // struct_block_vector
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
   Parser ::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
   Parser ::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YYUSE (yykind);
        yyo << ')';
      }
  }
#endif

  void
   Parser ::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
   Parser ::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
   Parser ::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
   Parser ::debug_stream () const
  {
    return *yycdebug_;
  }

  void
   Parser ::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


   Parser ::debug_level_type
   Parser ::debug_level () const
  {
    return yydebug_;
  }

  void
   Parser ::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

   Parser ::state_type
   Parser ::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
   Parser ::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
   Parser ::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
   Parser ::operator() ()
  {
    return parse ();
  }

  int
   Parser ::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_simple_type: // simple_type
        yylhs.value.emplace< enum class simple_item_type_E > ();
        break;

      case symbol_kind::S_NAME: // NAME
      case symbol_kind::S_ENUM_VALUE: // ENUM_VALUE
      case symbol_kind::S_VALUE: // VALUE
      case symbol_kind::S_HEXVALUE: // HEXVALUE
      case symbol_kind::S_FVALUE: // FVALUE
      case symbol_kind::S_STRING_CONSTANT: // STRING_CONSTANT
      case symbol_kind::S_CHAR_CONSTANT: // CHAR_CONSTANT
      case symbol_kind::S_DOC_COMMENT: // DOC_COMMENT
      case symbol_kind::S_name: // name
      case symbol_kind::S_simple_default_value: // simple_default_value
      case symbol_kind::S_value: // value
      case symbol_kind::S_struct_block_vector: // struct_block_vector
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 29: // name: NAME
#line 142 "struct_yacc.y"
                                                                                                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 783 "Parser.cpp"
    break;

  case 31: // enum_start: enum NAME
#line 146 "struct_yacc.y"
                                                                { info_items->process_enum(yystack_[0].value.as < std::string > (), false); }
#line 789 "Parser.cpp"
    break;

  case 32: // enum_start: enum NAME DOC_COMMENT
#line 147 "struct_yacc.y"
                                                                            { info_items->process_enum(yystack_[1].value.as < std::string > (), false, yystack_[0].value.as < std::string > ()); }
#line 795 "Parser.cpp"
    break;

  case 33: // enum_start: enum class name
#line 148 "struct_yacc.y"
                                                                            { info_items->process_enum(yystack_[0].value.as < std::string > (), true); }
#line 801 "Parser.cpp"
    break;

  case 34: // enum_start: enum class name DOC_COMMENT
#line 149 "struct_yacc.y"
                                                                            { info_items->process_enum(yystack_[1].value.as < std::string > (), true, yystack_[0].value.as < std::string > ()); }
#line 807 "Parser.cpp"
    break;

  case 39: // enum_ent: name
#line 158 "struct_yacc.y"
                                                                                            { info_items->process_enum_line(yystack_[0].value.as < std::string > (), false, "-");}
#line 813 "Parser.cpp"
    break;

  case 40: // enum_ent: name equals VALUE
#line 159 "struct_yacc.y"
                                                                            { info_items->process_enum_line(yystack_[2].value.as < std::string > (), true, yystack_[0].value.as < std::string > ());}
#line 819 "Parser.cpp"
    break;

  case 41: // enum_ent: name equals HEXVALUE
#line 160 "struct_yacc.y"
                                                                            { info_items->process_enum_line(yystack_[2].value.as < std::string > (), true, yystack_[0].value.as < std::string > ());}
#line 825 "Parser.cpp"
    break;

  case 42: // enum_ent: name equals CHAR_CONSTANT
#line 161 "struct_yacc.y"
                                                                            { info_items->process_enum_line(yystack_[2].value.as < std::string > (), true, yystack_[0].value.as < std::string > ());}
#line 831 "Parser.cpp"
    break;

  case 44: // struct_start: struct name
#line 165 "struct_yacc.y"
                                                                { info_items->process_struct(yystack_[0].value.as < std::string > ()); }
#line 837 "Parser.cpp"
    break;

  case 45: // struct_start: struct name DOC_COMMENT
#line 166 "struct_yacc.y"
                                                                { info_items->process_struct(yystack_[1].value.as < std::string > ()); }
#line 843 "Parser.cpp"
    break;

  case 48: // struct_block_ent: simple_type name semicolon
#line 171 "struct_yacc.y"
                                                                { info_items->process_struct_line_simple(yystack_[2].value.as < enum class simple_item_type_E > (), yystack_[1].value.as < std::string > (), "-");}
#line 849 "Parser.cpp"
    break;

  case 49: // struct_block_ent: simple_type name semicolon DOC_COMMENT
#line 172 "struct_yacc.y"
                                                                            { info_items->process_struct_line_simple(yystack_[3].value.as < enum class simple_item_type_E > (), yystack_[2].value.as < std::string > (), "-", yystack_[0].value.as < std::string > ());}
#line 855 "Parser.cpp"
    break;

  case 50: // struct_block_ent: simple_type name simple_default_value semicolon
#line 173 "struct_yacc.y"
                                                                            { info_items->process_struct_line_simple(yystack_[3].value.as < enum class simple_item_type_E > (), yystack_[2].value.as < std::string > (), yystack_[1].value.as < std::string > ());}
#line 861 "Parser.cpp"
    break;

  case 51: // struct_block_ent: simple_type name simple_default_value semicolon DOC_COMMENT
#line 174 "struct_yacc.y"
                                                                                       { info_items->process_struct_line_simple(yystack_[4].value.as < enum class simple_item_type_E > (), yystack_[3].value.as < std::string > (), yystack_[2].value.as < std::string > (), yystack_[0].value.as < std::string > ());}
#line 867 "Parser.cpp"
    break;

  case 52: // struct_block_ent: name name semicolon
#line 175 "struct_yacc.y"
                                                                            { info_items->process_struct_line_complex(complex_item_type_E::enum_or_struct_E, yystack_[2].value.as < std::string > (), yystack_[1].value.as < std::string > ()); }
#line 873 "Parser.cpp"
    break;

  case 53: // struct_block_ent: name name semicolon DOC_COMMENT
#line 176 "struct_yacc.y"
                                                                            { info_items->process_struct_line_complex(complex_item_type_E::enum_or_struct_E, yystack_[3].value.as < std::string > (), yystack_[2].value.as < std::string > (), yystack_[0].value.as < std::string > ()); }
#line 879 "Parser.cpp"
    break;

  case 54: // struct_block_ent: name name simple_default_value semicolon
#line 177 "struct_yacc.y"
                                                                            { info_items->process_struct_line_complex(complex_item_type_E::enum_or_struct_E, yystack_[3].value.as < std::string > (), yystack_[2].value.as < std::string > ()); }
#line 885 "Parser.cpp"
    break;

  case 55: // struct_block_ent: name name simple_default_value semicolon DOC_COMMENT
#line 178 "struct_yacc.y"
                                                                               { info_items->process_struct_line_complex(complex_item_type_E::enum_or_struct_E, yystack_[4].value.as < std::string > (), yystack_[3].value.as < std::string > (), yystack_[0].value.as < std::string > ()); }
#line 891 "Parser.cpp"
    break;

  case 56: // struct_block_ent: struct_block_vector name semicolon
#line 179 "struct_yacc.y"
                                                                            { info_items->process_struct_line_complex(complex_item_type_E::vector_E, yystack_[2].value.as < std::string > (), yystack_[1].value.as < std::string > ()); }
#line 897 "Parser.cpp"
    break;

  case 57: // struct_block_ent: struct_block_vector name semicolon DOC_COMMENT
#line 180 "struct_yacc.y"
                                                                            { info_items->process_struct_line_complex(complex_item_type_E::vector_E, yystack_[3].value.as < std::string > (), yystack_[2].value.as < std::string > (), yystack_[0].value.as < std::string > ()); }
#line 903 "Parser.cpp"
    break;

  case 58: // simple_type: bool
#line 182 "struct_yacc.y"
                                                                                                                { yylhs.value.as < enum class simple_item_type_E > () = simple_item_type_E::bool_E; }
#line 909 "Parser.cpp"
    break;

  case 59: // simple_type: int
#line 183 "struct_yacc.y"
                                                                                                                        { yylhs.value.as < enum class simple_item_type_E > () = simple_item_type_E::int_E; }
#line 915 "Parser.cpp"
    break;

  case 60: // simple_type: short
#line 184 "struct_yacc.y"
                                                                                                                    { yylhs.value.as < enum class simple_item_type_E > () = simple_item_type_E::short_E; }
#line 921 "Parser.cpp"
    break;

  case 61: // simple_type: long
#line 185 "struct_yacc.y"
                                                                                                                    { yylhs.value.as < enum class simple_item_type_E > () = simple_item_type_E::long_E; }
#line 927 "Parser.cpp"
    break;

  case 62: // simple_type: unsigned int
#line 186 "struct_yacc.y"
                                                                                                                { yylhs.value.as < enum class simple_item_type_E > () = simple_item_type_E::unsigned_int_E; }
#line 933 "Parser.cpp"
    break;

  case 63: // simple_type: unsigned short
#line 187 "struct_yacc.y"
                                                                                                            { yylhs.value.as < enum class simple_item_type_E > () = simple_item_type_E::unsigned_short_E; }
#line 939 "Parser.cpp"
    break;

  case 64: // simple_type: unsigned long
#line 188 "struct_yacc.y"
                                                                                                                { yylhs.value.as < enum class simple_item_type_E > () = simple_item_type_E::unsigned_long_E; }
#line 945 "Parser.cpp"
    break;

  case 65: // simple_type: float
#line 189 "struct_yacc.y"
                                                                                                                        { yylhs.value.as < enum class simple_item_type_E > () = simple_item_type_E::float_E; }
#line 951 "Parser.cpp"
    break;

  case 66: // simple_type: double
#line 190 "struct_yacc.y"
                                                                                                                    { yylhs.value.as < enum class simple_item_type_E > () = simple_item_type_E::double_E; }
#line 957 "Parser.cpp"
    break;

  case 67: // simple_type: std_string
#line 191 "struct_yacc.y"
                                                                                                            { yylhs.value.as < enum class simple_item_type_E > () = simple_item_type_E::std_string_E; }
#line 963 "Parser.cpp"
    break;

  case 68: // simple_default_value: block_start block_end
#line 193 "struct_yacc.y"
                                                                { yylhs.value.as < std::string > () = "{}"; }
#line 969 "Parser.cpp"
    break;

  case 69: // simple_default_value: block_start value block_end
#line 194 "struct_yacc.y"
                                                                            { yylhs.value.as < std::string > () = "{" + yystack_[1].value.as < std::string > () + "}"; }
#line 975 "Parser.cpp"
    break;

  case 70: // value: VALUE
#line 196 "struct_yacc.y"
                                                                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 981 "Parser.cpp"
    break;

  case 71: // value: FVALUE
#line 197 "struct_yacc.y"
                                                                    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 987 "Parser.cpp"
    break;

  case 72: // value: true
#line 198 "struct_yacc.y"
                                                                    { yylhs.value.as < std::string > () = "true"; }
#line 993 "Parser.cpp"
    break;

  case 73: // value: false
#line 199 "struct_yacc.y"
                                                                    { yylhs.value.as < std::string > () = "false"; }
#line 999 "Parser.cpp"
    break;

  case 74: // value: ENUM_VALUE
#line 200 "struct_yacc.y"
                                                                    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1005 "Parser.cpp"
    break;

  case 75: // value: STRING_CONSTANT
#line 201 "struct_yacc.y"
                                                                    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1011 "Parser.cpp"
    break;

  case 76: // value: CHAR_CONSTANT
#line 202 "struct_yacc.y"
                                                                    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1017 "Parser.cpp"
    break;

  case 77: // struct_block_vector: std_vector lessthan name greaterthan
#line 204 "struct_yacc.y"
                                                                { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > (); }
#line 1023 "Parser.cpp"
    break;


#line 1027 "Parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
   Parser ::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  const char *
   Parser ::symbol_name (symbol_kind_type yysymbol)
  {
    static const char *const yy_sname[] =
    {
    "end of file", "error", "invalid token", "{", "}", "=", ",", ";", "<",
  ">", "enum", "class", "struct", "int", "short", "long", "unsigned",
  "float", "double", "bool", "true", "false", "std::string", "std::vector",
  "NAME", "ENUM_VALUE", "VALUE", "HEXVALUE", "FVALUE", "STRING_CONSTANT",
  "CHAR_CONSTANT", "DOC_COMMENT", "$accept", "file_content", "parts",
  "part", "block_start", "block_end", "comma", "semicolon", "equals",
  "lessthan", "greaterthan", "true", "false", "unsigned", "int", "short",
  "long", "std_string", "std_vector", "enum", "class", "struct", "float",
  "double", "bool", "name", "enum_part", "enum_start", "enum_block",
  "enum_block_ents", "enum_ent", "struct_part", "struct_start",
  "struct_block_ents", "struct_block_ent", "simple_type",
  "simple_default_value", "value", "struct_block_vector", YY_NULLPTR
    };
    return yy_sname[yysymbol];
  }



  //  Parser ::context.
   Parser ::context::context (const  Parser & yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
   Parser ::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        int yychecklim = yylast_ - yyn + 1;
        int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }



  int
   Parser ::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
   Parser ::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char  Parser ::yypact_ninf_ = -49;

  const signed char  Parser ::yytable_ninf_ = -1;

  const signed char
   Parser ::yypact_[] =
  {
      -6,   -49,   -49,   -49,    13,   -49,    -6,    -4,    -3,   -49,
      14,   -49,    14,   -49,   -49,   -49,   -15,    -3,   -49,    -9,
     -49,    -3,   -49,    44,   -49,    -1,   -49,    26,    30,    35,
     -49,   -49,   -49,   -49,   -49,   -49,   -49,   -49,   -49,    24,
     -49,   -49,   -49,   -49,    37,   -49,   -49,   -49,    -3,    30,
      44,    -3,    -3,   -49,   -49,   -16,   -49,    40,   -49,    -3,
     -49,   -49,   -49,   -49,    -3,     5,    40,   -49,     5,    40,
     -49,   -49,   -49,   -49,   -49,   -49,    41,    -2,    20,    40,
     -49,    21,    40,    22,   -49,   -49,   -49,   -49,   -49,   -49,
     -49,   -49,   -49,   -49,   -49,   -49,    30,   -49,    23,   -49,
      33,   -49,   -49,   -49,   -49
  };

  const signed char
   Parser ::yydefact_[] =
  {
       0,    23,    25,     7,     0,     2,     3,     0,     0,     5,
       0,     6,     0,     1,     4,    24,    31,     0,    29,    44,
       8,     0,    30,     0,    32,    33,    45,    39,     0,    36,
      18,    19,    20,    17,    26,    27,    28,    21,    22,     0,
      59,    60,    61,    67,     0,    65,    66,    58,     0,     0,
      46,     0,     0,    34,    12,     0,     9,     0,    10,    38,
      62,    63,    64,    13,     0,     0,     0,    47,     0,     0,
      40,    41,    42,    11,    35,    37,     0,     0,    52,     0,
      43,    48,     0,    56,    14,    77,    15,    16,    74,    70,
      71,    75,    76,    68,    72,    73,     0,    53,    54,    49,
      50,    57,    69,    55,    51
  };

  const signed char
   Parser ::yypgoto_[] =
  {
     -49,   -49,    49,   -49,    -7,   -48,   -49,   -33,   -49,   -49,
     -49,   -49,   -49,   -49,    31,    32,    34,   -49,   -49,   -49,
     -49,   -49,   -49,   -49,   -49,    -8,   -49,   -49,   -49,     6,
     -49,   -49,   -49,    19,   -49,   -49,     4,   -49,   -49
  };

  const signed char
   Parser ::yydefgoto_[] =
  {
      -1,     4,     5,     6,    77,    57,    59,    74,    55,    64,
      85,    94,    95,    39,    40,    41,    42,    43,    44,     7,
      17,     8,    45,    46,    47,    27,     9,    10,    22,    28,
      29,    11,    12,    49,    50,    51,    79,    96,    52
  };

  const signed char
   Parser ::yytable_[] =
  {
      19,    66,    56,    21,     1,    23,     2,    15,    20,    25,
      70,    71,    73,    13,    72,    48,    24,    20,    86,    87,
      16,    18,    26,    88,    89,     3,    90,    91,    92,    93,
      53,    54,    78,    80,    56,    81,    83,    30,    31,    32,
      65,    58,    48,    68,    69,    63,    98,    73,   102,   100,
      84,    97,    99,   101,   103,    14,    76,    30,    31,    32,
      33,    34,    35,    36,   104,    75,    37,    38,    18,    67,
      60,    61,    82,    62
  };

  const signed char
   Parser ::yycheck_[] =
  {
       8,    49,     4,    10,    10,    12,    12,    11,     3,    17,
      26,    27,     7,     0,    30,    23,    31,     3,    20,    21,
      24,    24,    31,    25,    26,    31,    28,    29,    30,    77,
      31,     5,    65,    66,     4,    68,    69,    13,    14,    15,
      48,     6,    50,    51,    52,     8,    79,     7,    96,    82,
       9,    31,    31,    31,    31,     6,    64,    13,    14,    15,
      16,    17,    18,    19,    31,    59,    22,    23,    24,    50,
      39,    39,    68,    39
  };

  const signed char
   Parser ::yystos_[] =
  {
       0,    10,    12,    31,    33,    34,    35,    51,    53,    58,
      59,    63,    64,     0,    34,    11,    24,    52,    24,    57,
       3,    36,    60,    36,    31,    57,    31,    57,    61,    62,
      13,    14,    15,    16,    17,    18,    19,    22,    23,    45,
      46,    47,    48,    49,    50,    54,    55,    56,    57,    65,
      66,    67,    70,    31,     5,    40,     4,    37,     6,    38,
      46,    47,    48,     8,    41,    57,    37,    65,    57,    57,
      26,    27,    30,     7,    39,    61,    57,    36,    39,    68,
      39,    39,    68,    39,     9,    42,    20,    21,    25,    26,
      28,    29,    30,    37,    43,    44,    69,    31,    39,    31,
      39,    31,    37,    31,    31
  };

  const signed char
   Parser ::yyr1_[] =
  {
       0,    32,    33,    34,    34,    35,    35,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    59,    59,    59,    60,    61,    61,    61,    62,
      62,    62,    62,    63,    64,    64,    65,    65,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    68,    68,
      69,    69,    69,    69,    69,    69,    69,    70
  };

  const signed char
   Parser ::yyr2_[] =
  {
       0,     2,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     3,     3,     4,     4,     1,     3,     2,     1,
       3,     3,     3,     5,     2,     3,     1,     2,     3,     4,
       4,     5,     3,     4,     4,     5,     3,     4,     1,     1,
       1,     1,     2,     2,     2,     1,     1,     1,     2,     3,
       1,     1,     1,     1,     1,     1,     1,     4
  };




#if YYDEBUG
  const unsigned char
   Parser ::yyrline_[] =
  {
       0,    88,    88,    90,    91,    93,    94,    95,    97,    99,
     101,   103,   105,   107,   109,   111,   113,   115,   117,   119,
     121,   124,   126,   128,   130,   132,   134,   136,   138,   142,
     144,   146,   147,   148,   149,   151,   153,   154,   155,   158,
     159,   160,   161,   163,   165,   166,   168,   169,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   193,   194,
     196,   197,   198,   199,   200,   201,   202,   204
  };

  void
   Parser ::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
   Parser ::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

   Parser ::symbol_kind_type
   Parser ::yytranslate_ (int t)
  {
    return static_cast<symbol_kind_type> (t);
  }

} // yy
#line 1524 "Parser.cpp"

#line 206 "struct_yacc.y"


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

