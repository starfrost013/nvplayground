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
#define MAX_STR							260 

/* Logging system */
#define LOG_FILE_DEFAULT_NAME           "nvplay.log"   

typedef enum log_level 
{
    // A debug log message
    log_level_debug = 1,
    // A normal log message
    log_level_message = 1 << 1,
    // A warning
    log_level_warning = 1 << 2,
    // A fatal error
    log_level_error = 1 << 3,
} log_level;

// Enumerates (as a bitfield) the available logging destinations.
typedef enum log_dest_e
{
    log_dest_console = 1,
    log_dest_file = 1 << 1,
} log_dest; 

// Log redirection flags
// Determines which standard i/o should be redirected to the log file
typedef enum log_redirect_e
{
    log_redirect_none = 0,
    log_redirect_stdin = 1 << 1,                // Redirect STDIN to the log file
    log_redirect_stdout = 1 << 2,               // Redirect STDOUT to the log file
    log_redirect_stderr = 1 << 3,               // Redirect STDERR to the log file
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

//
// Commandline parser 
//

typedef struct command_line_s
{
    bool run_all_tests;             // Override test ini and run all tests
    bool use_test_ini;              // Use the test ini file
    bool dry_run;                   // don't run tests, but confirm the INI settings
    bool load_reg_script;           // run a registry script file
    bool load_replay_file;          // Load a replay file
    bool show_help;                 // Show a help message
    bool boot_only;                 // Boot the card and exit.
    char reg_script_file[MAX_STR];  // The registry script file to use
    char savestate_file[MAX_STR];   // The savestate file to use
    char replay_file[MAX_STR];      // The replay file to use

} command_line_t;

extern command_line_t command_line; 

//cannot use int32_t because it's defined as long
bool Cmdline_Parse(int argc, char** argv);

// String utils

bool String_IsEntirelyWhitespace(char* fmt, uint32_t max);
char* String_GetTokenSeparatedPart(char* fmt, const char* delim, uint32_t n);

char* String_LTrim(char* fmt, uint32_t max);
char* String_RTrim(char* fmt, uint32_t max);

// Input utils
// using a char may defy the scancode differences. 



#define KEYBOARD_MOD_RIGHT_SHIFT            (1 << 1)
#define KEYBOARD_MOD_LEFT_SHIFT             (1 << 2)
#define KEYBOARD_MOD_ANY_CTRL               (1 << 3)        /* Not recommended */
#define KEYBOARD_MOD_ANY_ALT                (1 << 4)
#define KEYBOARD_MOD_SCROLL_LOCK_ACTIVE     (1 << 5)
#define KEYBOARD_MOD_NUM_LOCK_ACTIVE        (1 << 6)
#define KEYBOARD_MOD_CAPS_LOCK_ACTIVE       (1 << 7)
#define KEYBOARD_MOD_LEFT_CTRL              (1 << 8)
#define KEYBOARD_MOD_LEFT_ALT               (1 << 9)
#define KEYBOARD_MOD_RIGHT_CTRL             (1 << 10)
#define KEYBOARD_MOD_RIGHT_ALT              (1 << 11)
#define KEYBOARD_MOD_SCROLL_LOCK_PRESSED    (1 << 12)
#define KEYBOARD_MOD_NUM_LOCK_PRESSED       (1 << 13)
#define KEYBOARD_MOD_CAPS_LOCK_PRESSED      (1 << 14)
#define KEYBOARD_MOD_SYSRQ                  (1 << 15)

bool Input_KeyDown(uint8_t scancode); 
bool Input_ModState(uint16_t mod_flags);