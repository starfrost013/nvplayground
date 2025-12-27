/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs 
    Licensed under the MIT license (see license file)

    util_logging.c: Logging implementation 
*/

#include "console/console.h"
#include "util.h"
#include <nvplay.h>

log_settings_t log_settings = {0};  

// Internal globals
FILE* log_file_stream = NULL; 

// Internal defines
#define LOG_STRING_BUF_SIZE     1024

bool Logging_Init()
{
    if (!log_settings.valid)
    {
        printf("BUG: didn't set up log_settings!\n");
        return false; 
    }

    if (!log_settings.level)
    {
        printf("Warning: No log channels specified. Nothing will be logged!\n");
        return true; 
    }

    // First try and open the log file. This truncates it if it doesn't exist
    if (log_settings.destination & LOG_DEST_FILE)
    {
        if (log_settings.file_name)
            log_file_stream = fopen(log_settings.file_name, "w+"); // open in text mode
        else
            log_file_stream = fopen(LOG_FILE_DEFAULT_NAME, "w+");
    }

    if (log_settings.redirect & LOG_REDIRECT_STDIN)
        freopen(log_settings.file_name, "w+", stdin);
    if (log_settings.redirect & LOG_REDIRECT_STDOUT)
        freopen(log_settings.file_name, "r", stdout);
    if (log_settings.redirect & LOG_REDIRECT_STDERR)
        freopen(log_settings.file_name, "w+", stderr);
    
    log_settings.open = true;

    return true; 
}

void Logging_Write(log_level level, const char* fmt, ...)
{
    if (!log_settings.open)
        return; 

    va_list ap = {0};
    
    const char* prefix = NULL;
    char log_string[LOG_STRING_BUF_SIZE] = {0};

    // ignore prefix if LOG_LEVEL_MESSAGE
    switch (level)
    {
        case LOG_LEVEL_DEBUG:
            prefix = "[DEBUG]: ";
            break;
        case LOG_LEVEL_MESSAGE:
            break; // not needed but shuts up the compiler    
        case LOG_LEVEL_WARNING:
            prefix = "[WARNING]: ";
            break;
        case LOG_LEVEL_ERROR:
            prefix = "[ERROR]: ";
            break;
    }

    // write out the prefix separately, it simplifies the code below
    if (prefix)
    {
        if (log_settings.destination & LOG_DEST_CONSOLE)
            fputs(prefix, stdout);

        if (log_settings.destination & LOG_DEST_FILE)
            fwrite(prefix, strlen(prefix), 1, log_file_stream);
    }

    va_start(ap, fmt);

    // full string printed out here so it can be sent to multiple places
    vsnprintf(log_string, LOG_STRING_BUF_SIZE, fmt, ap);

    // don't print a newline after
    if (log_settings.destination & LOG_DEST_CONSOLE)
    {
        if (nvplay_state.config.dumb_console)
            fputs(log_string, stdout);
        else 
            Console_PushLine(log_string);
    }

    if (log_settings.destination & LOG_DEST_FILE)
        fwrite(log_string, strlen(log_string), 1, log_file_stream);

    va_end(ap);

    if (log_settings.flush_on_line)
        fflush(log_file_stream);
}

void Logging_Shutdown()
{
    fclose(log_file_stream);
    log_settings.open = false;
}