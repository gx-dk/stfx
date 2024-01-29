stfx : Structs To/From XML
==========================

stfx is a program that translates a group of C++ structure and enum definitions and generates code to save the data to an XML file and restore an XML file into the data structure.

stfx was conceived for the task of loading and saving a configuration file, automating this task has been seen to be beneficial.

stfx generates code that can translate enum strings to enums and enums to enum strings. Code is also generated that allows fmt::format to handle enum parameters directly, converting instances of the enums to strings in the output.

Use
===
stfx takes one argument

	stfx <stfx_file_name>
		or
	stfx <header_file_name>
	
(Currently ONLY stfx <header_file_name> is implemented)

When stfx <header_file_name> is used, then the named header file is read and parsed. Four files are generated:
* structs.h
* structs.cpp
* enums.h
* enums.cpp

Restrictions on content of input header file
============================================
The input file parser parses a subset of C++.

Supported constructs are:
* enum
* enum class
* struct 
* simple variables with default constructors
* std::string
* std::vector

Functions and function headers are NOT supported by the input file parser

Pre-processor lines are IGNORED by the input file parser

Submodules used
================

* tinyxml2 : a simple, small, efficient, C++ XML parser https://github.com/leethomason/tinyxml2.git
* tinyxml2-ex : a set of add-on classes and helper functions for tinyxml2 https://github.com/stanthomas/tinyxml2-ex.git
* fmt : a formatting library providing a fast and safe alternative to C stdio and C++ iostreams. (Basis of std::format) https://github.com/fmtlib/fmt.git

building stfx
=============
The parser : lex_parse sub-directory of src
----------
The two files struct_lex.l and struct_yacc.y contain the input declaration. Use gnu flex and gnu bison to process these files (there is a script for use on windows "do_processing.bat" which is used to run these steps. Command line arguments should be the same when run on Unix style operating systems. 

The current output from win_flex and win_bison has been stored in this directory, to avoid the need to install/run flex and bison.

Building the executable
-----------------------
I use premake5 (https://premake.github.io/) to build the project / makefiles for stfx.

To build visual studio solution/project files use the command
	premake5 vs2019
For makefiles
	premake5 gmake2

I have included the generated solution/project file in the src directory

(at some point I will add in devops code to automatically create and release executables when changes are checked in). 
	