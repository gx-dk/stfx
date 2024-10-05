-- premake5.lua for building stfx program

function standard_configuration()
	filter { "Debug" }
		defines { "DEBUG" }
		objdir "debug/obj"
		targetdir "debug"
		symbols "on"

	-- filters related to RELEASE builds
	filter { "Release" }
		defines { "NDEBUG" }
		objdir "release/obj"
		targetdir "release"
		optimize "On"
		optimize "Speed"
end


dofile("submodules/premake5_submodules.lua")


solution "_stfx"
	configurations { "Debug", "Release" }
	
	project "stfx"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		
		files {"main.cpp", "*.cpp", "*.h" }
		includedirs {"."}
		files {"lex_parse/*.cpp", "lex_parse/*.hpp"}
		includedirs {"lex_parse"}
		files {"configuration/*.cpp", "configuration/*.h"}
		includedirs {"configuration"}
		files {"creation/**.cpp", "creation/**.h"}
		includedirs {"creation", "creation/*"}
		includedirs {"C:/ProgramData/chocolatey/lib/winflexbison3/tools"}	-- for <FlexLexer.h> 
		files {"lex_parse/*.y", "lex_parse/*.l"}

--		defines { "YY_NO_UNISTD_H"}
		defines {"__STDC_VERSION__=199901L"}	-- inform win_flex that we don't want multiple definitions of limits (from stdint.h)
		
		
	filter "system:Windows"
		defines { "WIN32"  }
		defines {" _CRT_SECURE_NO_DEPRECATE", "_CRT_NONSTDC_NO_DEPRECATE"}
	filter {}

		using_fmt(true)
		using_tinyxml2(true)
		using_tinyxml2_ex()
		standard_configuration()

	project "test"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		
		files {"test/test_main.cpp", "test/common_out/*.cpp", "test/common_out/*.h" }
		includedirs {"test", "test/common_out"}
		
		filter {}

		using_fmt(true)
		using_tinyxml2(true)
		using_tinyxml2_ex()
		standard_configuration()
