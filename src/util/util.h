/* Core utility headers 
*/

#pragma once
#include "ini.h"

/* Logging system */
#define LOGGING_FILE_DEFAULT_NAME "nvplay.log"   

typedef enum log_level_s 
{
    // A debug log message
    log_level_debug = 1,
    
    // A normal log message
    log_level_message = 1 << 1,

    // A warning
    log_level_warning = 1 << 2,

    // A fatal error
    log_level_error = 1 << 3,
} log_level_t;

typedef struct log_settings_s
{
    log_level_t log_level;
    bool valid;                 // Determines if the log settings were actually set,
} log_settings_t;

extern log_settings_t log_settings; 

bool Logging_Init();