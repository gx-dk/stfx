// enums.cpp
// created 2024-07-28 16:22:06.6503051
// Automatically generated using stfx. Do not directly edit this file, use stfx to re-create this file.
// Licence : MIT License

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
			{ serialization_type_E::ini_no_equals, "ini_no_equals" },
			{ serialization_type_E::json, "json" },
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
			{ "ini_no_equals", serialization_type_E::ini_no_equals },
			{ "json", serialization_type_E::json },
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

