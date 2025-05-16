#include "util/ini.h"
#include <config/config.h>

// Globals
nv_config_t config; 

// Functions
bool Config_Load()
{
    config.ini_file = ini_read(INI_FILE_NAME);

    if (!config.ini_file)
        return false; 

    ini_section_t section_tests = ini_find_section(config.ini_file, "Tests");
};