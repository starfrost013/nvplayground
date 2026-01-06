/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    gpu_repl.h: REPL mode headers
*/

#pragma once 

#include <nvplay.h>

#define MAX_COMMAND_HISTORY     10

typedef struct repl_command_history_entry_s
{
    char cmd[MAX_STR];
} repl_command_history_entry_t;

extern int32_t command_history_id;
extern repl_command_history_entry_t command_history[MAX_COMMAND_HISTORY];

void NVPlay_Repl(); 