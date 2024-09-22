// test_main.cpp

#include "config_data.h"
#include "co_enums.h"
#include "co_structs.h"

int main(int argc, char *argv[])
	{
	config conf;



	;
	xml_writer_C rdwr(false);

	rdwr.write_to_file("C:\\tmp\\test.xml", conf);
	rdwr.read_from_file("C:\\tmp\\test1.xml", conf);
	}

