/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    util.h: Core utility headers
*/

#pragma once
#include <util/util_ini.h>
#include <util/util_scancodes.h>

/* General definitions */

/* Logging system */
#define LOG_FILE_DEFAULT_NAME           "nvplay.log"   

typedef enum log_level 
{
    // A debug log message
    LOG_LEVEL_DEBUG = 1,
    // A normal log message
    LOG_LEVEL_MESSAGE = 1 << 1,
    // A warning
    LOG_LEVEL_WARNING = 1 << 2,
    // A fatal error
    LOG_LEVEL_ERROR = 1 << 3,
} log_level;

// Enumerates (as a bitfield) the available logging destinations.
typedef enum log_dest_e
{
    LOG_DEST_CONSOLE = 1,
    LOG_DEST_FILE = 1 << 1,
} log_dest; 

// Log redirection flags
// Determines which standard i/o should be redirected to the log file
typedef enum log_redirect_e
{
    LOG_REDIRECT_NONE = 0,
    LOG_REDIRECT_STDIN = 1 << 1,                // Redirect STDIN to the log file
    LOG_REDIRECT_STDOUT = 1 << 2,               // Redirect STDOUT to the log file
    LOG_REDIRECT_STDERR = 1 << 3,               // Redirect STDERR to the log file
} log_redirect; 

typedef struct log_settings_s
{
    log_level level;
    log_dest destination; 
    const char* file_name; 
    bool valid;                 // Determines if the log settings were actually set up
    bool open;                  // Determines if the log is actually open
    bool flush_on_line;         // Determines if we should flush every single line.
    log_redirect redirect;      // Redirection flags
} log_settings_t;

extern log_settings_t log_settings; 

bool Logging_Init();
void Logging_Write(log_level level, const char* fmt, ...);
void Logging_Shutdown();

// String utils

bool String_IsEntirelyWhitespace(char* fmt, uint32_t max);
char* String_GetTokenSeparatedPart(char* fmt, const char* delim, uint32_t n);

char* String_LTrim(char* fmt, uint32_t max);
char* String_RTrim(char* fmt, uint32_t max);
