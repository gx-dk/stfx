// timestamp.cpp
// if the output file is deleted as part of a build, then a new timestamp is generated ...

#include "version.h"
#include "timestamp.h"

#ifndef VER_FILE_VERSION_STR
#define VER_FILE_VERSION_STR "fill this in... "
#endif


const char *app_timestamp = "stfx built at : " __DATE__ " " __TIME__ ", Software Version : " VER_FILE_VERSION_STR;


