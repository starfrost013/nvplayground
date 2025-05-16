//
// Config.h: Config Subsystem
//

#pragma once 

#include <nvplayground.h>
#include <core/tests/tests.h>
#include <util/util.h>

#define INI_FILE_NAME "nvplay.ini"

// This is a linked list.
// We can make a lot of simplifications by making some assumptions about or design; 
// e.g. we don't need to ever remove these since the enabled tests are enumerated at init.

typedef struct nv_config_test_entry_s
{
    const char* name; 
    struct nv_config_test_entry_s* prev; 
    struct nv_config_test_entry_s* next; 

} nv_config_test_entry_t; 


// Main config struct 
typedef struct nv_config_s
{
    bool loaded;                                    // Determines if the configuration file has been loaded.
    ini_t ini_file;
    nv_config_test_entry_t* first_test_entry;       // The first test entry.
} nv_config_t;

extern nv_config_t config; 

bool Config_Load();