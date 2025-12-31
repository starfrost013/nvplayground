/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    gpu_script_parser.c: Parses our very basic command/scripting system (mostly just for writing to registers.)
*/

#include <stdio.h>


#include "util/util.h"
#include <nvplay.h>
#include <config/config.h>

/* This is probably not threadsafe */
char last_command[MAX_STR] = {0};
/* This is so we can do stuff like strtok */
char last_token[MAX_STR] = {0};

uint32_t Command_Argc()
{
    // restore the original last command so we can parse it

    strncpy(last_token, last_command, MAX_STR);
	uint32_t amount = 0;

	// skip the command name

	char* tok = strtok(NULL,  " ");

	while (tok)
	{
		amount++;
		tok = strtok(NULL, " ");
	}

	return amount; 
}

// Return parameter "argv" of command "command".
const char* Command_Argv(uint32_t argv)
{
    // restore the original last command so we can parse it
    strncpy(last_token, last_command, MAX_STR);

	char* str = String_GetTokenSeparatedPart(last_token, " ", argv);

	if (!str)
		return STRING_EMPTY;

	return str; 
}

void Script_RunCommand(char* line_buf)
{
	// skip blank lines if they exist
	if (String_IsEntirelyWhitespace(line_buf, MAX_STR))
		return; 

	// trim any whitespace
	// todo: string_rtrim (after the test rectangle script is verified to work.)
	char* line_buf_trimmed = String_LTrim(String_RTrim(line_buf, MAX_STR), MAX_STR);

	// skip commented lines
	if (line_buf_trimmed[0] == '/'
	&& line_buf_trimmed[1] == '/')
		return; 

	Logging_Write(log_level_debug, "Trimmed command string: %s\n", line_buf, line_buf_trimmed);

	strncpy(last_command, line_buf_trimmed, MAX_STR);

	// this gets the command
	char* command_name = strtok(line_buf_trimmed, " ");

	int32_t script_command_id = 0;
	gpu_script_command_t* script_command = &commands[script_command_id];

	bool command_valid = false; 
	bool command_found = false; 

	// just use a valid abbreviated name
	while (script_command->name_abbrev)
	{
		if (!strncmp(script_command->name_full, command_name, MAX_STR)
		|| !strncmp(script_command->name_abbrev, command_name, MAX_STR))
		{
			// store one for found and one for valid
			command_found = true; 
			command_valid = true; 

			// command code uses Command_Argc and Command_Argv to get functions	
			// don't terminate processing if a function pointer is not present though
			if (!script_command->function)
			{
				Logging_Write(log_level_warning, "Command %s has no function!\n", script_command->name_full);
				command_valid = false; 
			}

			if (Command_Argc() < script_command->num_parameters)
			{
				Logging_Write(log_level_warning, "Command %s does not have enough parameters!\n", script_command->name_full);
				command_valid = false; 
			}

			if (command_found
			&& command_valid)
			{
				if (!script_command->function())
					Logging_Write(log_level_error, "Command %s failed to execute!\n", script_command->name_full);

				// keep command_valid true
			}

		}

		// increment command id
		script_command_id++;
		script_command = &commands[script_command_id];
	}

	if (!command_valid && !command_found)
		Logging_Write(log_level_warning, "Unknown command %s\n", last_token);
}

void Script_Run()
{
	FILE* script_file = fopen(command_line.reg_script_file, "rb+");
	char line_buf[MAX_STR] = {0};

	if (!script_file)
	{
		Logging_Write(log_level_error, "Couldn't open script file %s\n", script_file);
		exit(7);
	}

	Logging_Write(log_level_message, "Running script file %s\n", command_line.reg_script_file);

	while (!feof(script_file))
	{
		// start a new line
		fgets(line_buf, MAX_STR, script_file);

		Script_RunCommand(line_buf);
	}
}