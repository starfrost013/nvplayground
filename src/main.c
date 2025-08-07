/* 
    NVPlay
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    main.c: Runs main function and test mode
*/

#include "config/config.h"
#include "util/util.h"
#include <nvplayground.h>
#include <config/config.h>
#include <stdio.h>

#define GDB_IMPLEMENTATION
#include "gdbstub.h"


void NVPlay_RunTests()
{
	Logging_Write(log_level_message, "GPU test mode\n");

	if (config.num_tests_enabled == 0)
	{
		Logging_Write(log_level_warning, "No tests to run. Exiting...\n");
		exit(5);
	}

	Logging_Write(log_level_message, "Running %ld tests...\n", config.num_tests_enabled);

	/* run each loaded test in order */
	nv_config_test_entry_t* current_entry = config.test_list_head; 

	uint32_t tests_succeeded = 0, tests_failed = 0;

	while (current_entry)
	{
		/* First check for dry-run */
		if (!command_line.dry_run)
		{
			/* 
				TODO: Ini setting to disable this print in the case of graphical tests.
				Otherwise we'll have to switch back to test mode every test.

				Also, test logging. (after util_logging.c is done)
			*/
			if (current_entry->test_function)
			{
				bool success = current_entry->test_function();	

				if (success)
				{
					tests_succeeded++;
					Logging_Write(log_level_message, "Test %s succeeded\n", current_entry->name);

				}
				else
				{
					tests_failed++;
					Logging_Write(log_level_message, "Test %s failed! :(\n", current_entry->name);

				}
			}
		}
		else
		{
			Logging_Write(log_level_message, "[DRY RUN - SKIP]\n");
		}
			
		current_entry = current_entry->next; 
	}

	Logging_Write(log_level_message, "%s: %lu tests ran, %lu/%lu succeeded (%lu failed)", 
		current_device.device_info.name, config.num_tests_enabled, tests_succeeded, config.num_tests_enabled, tests_failed);
}



void NVPlay_Run()
{
/* Make sure the GPU is supported */
	if (!current_device.device_info.init_function)
	{
		Logging_Write(log_level_error, "This GPU is not yet supported :(\n");
		exit(3);
	}

	if (!current_device.device_info.init_function())
	{
		Logging_Write(log_level_error, "GPU initialisation failed\n");
		exit(4);
	}	


	if (command_line.load_reg_script)
		Script_Run();
	else if (command_line.load_savestate_file)
		GPUS_Load();
	else
	{
		/* If main_function is set, call it, otherwise run the tests from the INI */
		if (current_device.device_info.main_function)
		{
			Logging_Write(log_level_message, "Main function mode\n");

			current_device.device_info.main_function();
		}
		else 
		{
			NVPlay_RunTests();
		}
	}
}

void NVPlay_Shutdown()
{
	if (current_device.device_info.shutdown_function)
		current_device.device_info.shutdown_function();

	Logging_Shutdown();
	exit(0);
}

void NVPlay_ShowHelpAndExit()
{
	printf("%s", msg_help); // Done this way to shut up GCC
	NVPlay_Shutdown();
}

int main(int argc, char** argv) 
{
	_gdb_start(); // gdb_start but it doesn't actually break into the debugger automatically

	Cmdline_Parse(argc, argv);

	log_settings.destination = (log_dest_file | log_dest_console);
	log_settings.flush_on_line = true; //bad idea?
	log_settings.level = (log_level_debug | log_level_message | log_level_warning | log_level_error);
	log_settings.valid = true;
	
	if (!Logging_Init())
	{
		printf("Failed to initialise logging system\n");
		exit(7);
	}

	Logging_Write(log_level_message, APP_SIGNON_STRING);

	// early return
	if (command_line.show_help)
	{
		NVPlay_ShowHelpAndExit();
		return 0;
	}

	if (!pci_bios_is_present())
		exit(1);

	if (!detect_gpu())
		exit(2);

	Config_Load(); 

	NVPlay_Run();

	NVPlay_Shutdown();

 	return 0;
}