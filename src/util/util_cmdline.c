/* 
    NVPlayground
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    util_cmdline.c: Command line implementation
*/

#include <string.h>
#include <util/util.h>

command_line_t command_line = {0};

#define COMMAND_LINE_RUN_ALL "-a"
#define COMMAND_LINE_RUN_ALL_FULL "-all"
#define COMMAND_LINE_DRY_RUN "-d"
#define COMMAND_LINE_DRY_RUN_FULL "-dry"
#define COMMAND_LINE_RUN_SCRIPT_FILE "-s"
#define COMMAND_LINE_RUN_SCRIPT_FILE_FULL "-script"

bool Cmdline_Parse(int argc, char** argv)
{
    if (!argc)
        return true; 

    // there is at least 1 argument

    for (int32_t i = 0; i < argc; i++)
    {
        // normalise the case of the strings 
        
        char* current_arg = argv[i];
        char* next_arg = argv[i + 1];

        // use strcasecmp since it exists here
        if (!strcasecmp(current_arg, COMMAND_LINE_RUN_ALL)
        || !strcasecmp(current_arg, COMMAND_LINE_RUN_ALL_FULL))
        {
            command_line.run_all_tests = true; 
        }
        else if (!strcasecmp(current_arg, COMMAND_LINE_DRY_RUN)
        || !strcasecmp(current_arg, COMMAND_LINE_DRY_RUN_FULL))
        {
            command_line.dry_run = true; 
        }
        else if (!strcasecmp(current_arg, COMMAND_LINE_RUN_SCRIPT_FILE)
        || !strcasecmp(current_arg, COMMAND_LINE_RUN_SCRIPT_FILE_FULL))
        {
            // logging not yet initialised
            if (argc - i < 1)
                printf("No script file provided!\n");
            
            command_line.reg_script = true;
            strncpy(command_line.reg_script_file, next_arg, MAX_STR);
        
            //skip script file
            i++;
        }
    }

    return true; 
    
}