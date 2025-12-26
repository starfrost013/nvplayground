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
    const char* cmd;
    struct repl_command_history_entry_s* prev;
    struct repl_command_history_entry_s* next;
} repl_command_history_entry_t;

extern uint32_t command_history_id;

void NVPlay_Repl(); 