/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    gpu_repl.c: Implements the Read-Eval-Print loop for gpu i/o
*/

#include "util/util.h"
#include <nvplay.h>

#define COMMAND_EXIT            "q"
#define COMMAND_EXIT_VERBOSE    "exit"

#define COMMAND_HELP            "?"
#define COMMAND_HELP_VERBOSE    "help"

bool repl_is_running = true; 
uint32_t command_history_id = 0;

void NVPlay_ReplHelp()
{
    Logging_Write(LOG_LEVEL_MESSAGE, "%s", msg_help_script);
}


void NVPlay_Repl()
{
    char repl_string[MAX_STR] = {0};

    Logging_Write(LOG_LEVEL_MESSAGE, "Welcome to NVPlay\n");
    Logging_Write(LOG_LEVEL_MESSAGE, "Enter commands to interface with the GPU. Type ? or help for help. Type q or exit to exit.\n");

    while (repl_is_running)
    {
        // Update console
        //if (!nvplay_state.config.dumb_console)
            //Console_Update();

        bool input_recv = false; 

        Logging_Write(LOG_LEVEL_MESSAGE, "GPU>");

        if (!nvplay_state.config.dumb_console)
        {
            // TODO: Better input handling
            while (!input_recv)
            {
                input_recv = Input_GetString(repl_string, MAX_STR);
            }
        }
        else
        {
            input_recv = true; // fake it for dumb console mode
            fgets(repl_string, MAX_STR, stdin);
        }

         // fgets above blocks on dumbconsole, on curses, it doesn't
        if (input_recv)
        {
            Logging_Write(LOG_LEVEL_DEBUG, "repl_string = %s\n", repl_string);
            // get rid of the newline (could call String_GetRTrim(String_GetLTrim) but that does a lot of unnecessary stuff we don't need yet)
            repl_string[strcspn(repl_string, "\r\n")] = '\0';

            // gcc only so it is fine
            if (!strcasecmp(repl_string, COMMAND_EXIT)
            || !strcasecmp(repl_string, COMMAND_EXIT_VERBOSE))
            {
                repl_is_running = false; 
            }
            else if (!strcasecmp(repl_string, COMMAND_HELP)
            || !strcasecmp(repl_string, COMMAND_HELP_VERBOSE))
            {
                NVPlay_ReplHelp();
            }
            else 
                NVPlay_RunScriptCommand(repl_string);    
        }
    
    }
}