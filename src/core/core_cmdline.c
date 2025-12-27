/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    util_cmdline.c: Command line implementation
*/

#include "nvplay.h"
#include <string.h>
#include <util/util.h>


#define COMMAND_LINE_RUN_TEST_INI "-t"
#define COMMAND_LINE_RUN_TEST_INI_FULL "-test"
#define COMMAND_LINE_RUN_ALL "-a"
#define COMMAND_LINE_RUN_ALL_FULL "-all"
#define COMMAND_LINE_DRY_RUN "-d"
#define COMMAND_LINE_DRY_RUN_FULL "-dry"
#define COMMAND_LINE_RUN_SCRIPT_FILE "-s"
#define COMMAND_LINE_RUN_SCRIPT_FILE_FULL "-script"
#define COMMAND_LINE_LOAD_REPLAY "-nvr"
#define COMMAND_LINE_LOAD_REPLAY_FULL "-replay"
#define COMMAND_LINE_HELP "-?"
#define COMMAND_LINE_HELP_FULL "-help"
#define COMMAND_LINE_BOOTONLY "-b"
#define COMMAND_LINE_BOOTONLY_FULL "-bootonly"

// C23 constexpr pls
#define ARG_LEFT    argc - i < 1

bool NVPlay_ParseCmdline(int argc, char** argv)
{
    if (!argc)
        return true; 

    // there is at least 1 argument

    for (int32_t i = 0; i < argc; i++)
    {
        // normalise the case of the strings 
        
        char* current_arg = argv[i];
        char* next_arg = argv[i + 1];

        if (!strcasecmp(current_arg, COMMAND_LINE_RUN_SCRIPT_FILE)
        || !strcasecmp(current_arg, COMMAND_LINE_RUN_SCRIPT_FILE_FULL))
        {
            // logging not yet initialised
            if (ARG_LEFT)
            {
                printf("-script provided, but no registry script file provided!\n");
                return false;
            }
            
            nvplay_state.run_mode = NVPLAY_MODE_SCRIPT;
            strncpy(nvplay_state.reg_script_file, next_arg, MAX_STR);
        
            //skip script file
            i++;
        }
        else if (!strcasecmp(current_arg, COMMAND_LINE_LOAD_REPLAY)
        || !strcasecmp(current_arg, COMMAND_LINE_LOAD_REPLAY_FULL))
        {
            if (ARG_LEFT)
            {
                printf("-replay provided, but no replay file provided!\n");
                return false; 
            }

            nvplay_state.run_mode = NVPLAY_MODE_REPLAY;
            strncpy(nvplay_state.replay_file, next_arg, MAX_STR);
        }
        // help
        else if (!strcasecmp(current_arg, COMMAND_LINE_HELP)
        || !strcasecmp(current_arg, COMMAND_LINE_HELP_FULL))
        {
            nvplay_state.show_help = true; 
        }
        else if (!strcasecmp(current_arg, COMMAND_LINE_BOOTONLY)
        || !strcasecmp(current_arg, COMMAND_LINE_BOOTONLY_FULL))
        {
            // Maybe make it so we can load custom INI files?
            nvplay_state.run_mode = NVPLAY_MODE_BOOTGPU;
        }  
    }

    return true; 
    
}