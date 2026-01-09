/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    gpu_repl.c: Implements the Read-Eval-Print loop for gpu i/o
*/

#include "gpu_repl.h"
#include "curses.h"
#include "core/console/console.h"
#include "util/util.h"
#include <nvplay.h>

// Commands
#define COMMAND_EXIT            "q"
#define COMMAND_EXIT_VERBOSE    "exit"

#define COMMAND_HELP            "?"
#define COMMAND_HELP_VERBOSE    "help"

bool repl_is_running = true; 
int32_t command_history_id = 0;

// The command history starts at 0, reaches its max as you write commmands and then becomes fully full and one is always cleared.
int32_t command_history_id_max = 0;
repl_command_history_entry_t command_history[MAX_COMMAND_HISTORY] = {0};

void NVPlay_ReplHelp()
{
    Logging_Write(LOG_LEVEL_MESSAGE, "%s", msg_help_script);
}

void NVPlay_ReplIncrementCommandHistory()
{
    /*
    // overwrite whatever the user typed with the new command history
    size_t seek_length = strlen(command_history[command_history_id].cmd);
    int32_t x = 0, y = 0;

    Console_GetPosition(&x, &y);
    x -= seek_length;

    if (x < 0)
        x = 0;

    move(x, y);
*/


    Console_PushLine(command_history[command_history_id].cmd);

    command_history_id++;

    if (command_history_id >= command_history_id_max)
        command_history_id = (command_history_id_max - 1);

    Logging_Write(LOG_LEVEL_MESSAGE, "Console history is now %d (last command: %s)\n", command_history_id, command_history[command_history_id - 1].cmd);
}

void NVPlay_ReplDecrementCommandHistory()
{
    /*
    // overwrite whatever the user typed with the new command history
    size_t seek_length = strlen(command_history[command_history_id].cmd);
    int32_t x = 0, y = 0;

    Console_GetPosition(&x, &y);
    x -= seek_length;

    if (x < 0)
        x = 0;

    move(x, y);
*/
    Console_PushLine(command_history[command_history_id].cmd);

    command_history_id--;

    if (command_history_id < 0)
        command_history_id = 0;

    Logging_Write(LOG_LEVEL_MESSAGE, "Console history is now %d (last command: %s)\n", command_history_id, command_history[command_history_id].cmd);
}

void NVPlay_ReplOnCommandHistoryFull()
{
    // too much memory use?
    for (uint32_t i = 1; i < MAX_COMMAND_HISTORY; i++)
        memcpy((void*)&command_history[i - 1], (void*)&command_history[i], sizeof(repl_command_history_entry_t));

    // decrement command history id
    command_history_id--;
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
                int32_t last_char = -1;
                input_recv = Input_GetStringAndChar(repl_string, MAX_STR, &last_char);

                /* Yes it's inverted. Newer commands are actually at the "end" of the ring buffer*/
                switch (last_char)
                {
                    case KEY_UP:
                        NVPlay_ReplDecrementCommandHistory();
                        break;
                    case KEY_DOWN:
                        NVPlay_ReplIncrementCommandHistory();
                        break; 
                }
            }
        }
        else
            fgets(repl_string, MAX_STR, stdin);

        // get rid of the newline (could call String_GetRTrim(String_GetLTrim) but that does a lot of unnecessary stuff we don't need yet)
        repl_string[strcspn(repl_string, "\r\n")] = '\0';
        
        strncpy(command_history[command_history_id].cmd, repl_string, MAX_STR);

         // fgets above blocks on dumbconsole, on curses, it doesn't
        command_history_id++;

        // make sure this actually goes up to 10
        if (command_history_id > command_history_id_max)
            command_history_id_max = command_history_id;

        if (command_history_id >= MAX_COMMAND_HISTORY)
            NVPlay_ReplOnCommandHistoryFull();

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