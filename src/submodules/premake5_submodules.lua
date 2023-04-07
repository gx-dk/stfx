-- premake5_submodules.lua



function using_fmt(with_source)
	fmt_path = "submodules/fmt/"

	includedirs {fmt_path .. "include"}
	if(with_source == true)
	then
		files {fmt_path .. "include/fmt/format.h"}
		files {fmt_path .. "src/format.cc"}
	end
end

function using_tinyxml2(with_source)
	tinyxml2_path = "submodules/tinyxml2/"

	includedirs {tinyxml2_path}
	if(with_source == true)
	then
		files {tinyxml2_path .. "tinyxml2.h"}
		files {tinyxml2_path .. "tinyxml2.cpp"}
	end
end

function using_tinyxml2_ex()
	tinyxml2_ex_path = "submodules/tinyxml2-ex/"

	includedirs {tinyxml2_ex_path}
	files {tinyxml2_ex_path .. "*.h"}
	end