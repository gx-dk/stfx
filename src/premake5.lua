-- premake5.lua for building stfx program

dofile("C:/ProgramData/premaker/premake5_using.lua")

solution "_stfx"
	configurations { "Debug", "Release" }
	
		project "stfx"
		kind "ConsoleApp"
		language "C"
		language "C++"
		cppdialect "C++17"
		files {"main.cpp"}
		files { "*.cpp", "*.h" }
		files {"lex_parse/*.cpp", "lex_parse/*.hpp"}
		includedirs {"lex_parse"}
		defines { "win32"  }
--		defines { "YY_NO_UNISTD_H"}
		defines {"__STDC_VERSION__=199901L"}	-- inform win_flex that we don't want multiple definitions of limits (from stdint.h)
		defines {" _CRT_SECURE_NO_DEPRECATE", "_CRT_NONSTDC_NO_DEPRECATE"}
		includedirs {"."}
		includedirs {"C:/ProgramData/chocolatey/lib/winflexbison3/tools"}
		files {"lex_parse/*.y", "lex_parse/*.l"}
		using_fmt(true)
		standard_configuration()


