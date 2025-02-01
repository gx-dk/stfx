// enums.cpp
//
// Automatically generated using stfx
// Do not directly edit this file, use stfx to re-create this file.
//
// License : MIT License
//
// stfx built at : Feb  1 2025 04:31:50, Software Version : 25.01

#include "enums.h"

#include <string>
#include <map>

#include "config_data.h"

const std::string stfx::s_from_e(serialization_type_E e)
	{
	std::map < serialization_type_E, std::string > serialization_type_E_to_string =
		{
			{ serialization_type_E::xml_attrib, "xml_attrib" },
			{ serialization_type_E::xml, "xml" },
			{ serialization_type_E::ini, "ini" },
			{ serialization_type_E::n_ini, "n_ini" },
			{ serialization_type_E::json, "json" },
			{ serialization_type_E::argv, "argv" },
		};
	std::string rv = (serialization_type_E_to_string.find(e) != serialization_type_E_to_string.end()) ? serialization_type_E_to_string[e] : std::to_string(int(e));
	return rv;
	};

bool stfx::s_to_e(std::string s, serialization_type_E &e)
	{
	bool rv {false};

	std::map < std::string, serialization_type_E > string_to_serialization_type_E =
		{
			{ "xml_attrib", serialization_type_E::xml_attrib },
			{ "xml", serialization_type_E::xml },
			{ "ini", serialization_type_E::ini },
			{ "n_ini", serialization_type_E::n_ini },
			{ "json", serialization_type_E::json },
			{ "argv", serialization_type_E::argv },
		};
	if(string_to_serialization_type_E.find(s) != string_to_serialization_type_E.end())
		{
		e = string_to_serialization_type_E[s];
		rv = true;
		}
	else
		{
		try
			{
			int i = std::stoi(s);		// just in case the value of enum has been given
			e = serialization_type_E(i);
			}
		catch (std::exception &)
			{
			// unexpected string, will just be ignored (for now!)... This *COULD* throw an exception .. 
			}
		// note rv NOT set to true ! 
		}
	return rv;
	};

