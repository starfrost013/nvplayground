/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    script.h: Script parsers
*/

#pragma once
#include <nvplay.h>


void NVPlay_RunScript();
void NVPlay_RunScriptCommand(char* line_buf);

// This sucks. It's not a proper lexer/tokeniser, but we don't need one
typedef struct gpu_script_command_s
{
	const char* name_abbrev;
	const char* name_full;		// don't really need an alias system
	bool (*function)();
	uint32_t num_parameters;		// for parameter size checking
} gpu_script_command_t; 

extern gpu_script_command_t commands[];

/* Command utility stuff */
const char* Command_Argv(uint32_t argv);
uint32_t Command_Argc();
