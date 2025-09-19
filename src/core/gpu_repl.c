/* 
    NVPlay
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    gpu_repl.c: Implements the Read-Eval-Print loop for gpu i/o
*/

#include "util/util.h"
#include <nvplay.h>

bool repl_is_running = true; 

void GPURepl_Run()
{
    char repl_string[MAX_STR] = {0};

    Logging_Write(log_level_message, "Welcome to NVPlay\n");
    Logging_Write(log_level_message, "Type supported GPU commands. Type q or exit to exit.\n");

    while (repl_is_running)
    {
        // just raw printf
        printf("gpu>");

        // paranoid version of using scanf for this with a strictly limited size
        fgets(repl_string, MAX_STR, stdin);
        
        // gcc only so it is fine
        if (!strcasecmp(repl_string, "q")
        || !strcasecmp(repl_string, "exit"))
        {
            repl_is_running = false; 
        }
        else 
        {
            Script_RunCommand(repl_string);        
        }
    }
}