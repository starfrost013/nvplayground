/* 
    NVPlay
    Copyright © 2025-2026 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    main.c: Runs main function and test mode
*/

#include "config/config.h"
#include "nvplay.h"
#include "util/util.h"
#include <nvplay.h>
#include <cmake/nvplay_version.h>
#include <config/config.h>
#include <stdio.h>

#define GDB_IMPLEMENTATION
#include "gdbstub.h"

nvplay_state_t nvplay_state;

void NVPlay_RunTests()
{
	Logging_Write(log_level_message, "GPU test mode\n");

	if (config.num_tests_enabled == 0)
	{
		Logging_Write(log_level_warning, "No tests to run. Exiting...\n");
		NVPlay_Shutdown(NVPLAY_EXIT_CODE_NO_TESTS);
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
				TODO: Ini setting to disable this print in the case of graphical tests. Otherwise we'll have to switch back to test mode every test.

				Also, test logging. (after util_logging.c is done)
			*/

			bool success = Test_Run(current_entry);

			if (!success)
				tests_succeeded++;
			else
			 	tests_failed++;

			if (success)
			{
				tests_succeeded++;
				Logging_Write(log_level_message, "Test %s succeeded\n", current_entry->test->name);
			}
			else 
			{
				tests_failed++;
				Logging_Write(log_level_message, "Test %s failed! :(\n", current_entry->test->name);
			}
		}
		else
			Logging_Write(log_level_message, "[DRY RUN - SKIP]\n");

		current_entry = current_entry->next; 
	}

	Logging_Write(log_level_message, "%s: %lu tests ran, %lu/%lu succeeded (%lu failed)", 
		current_device.device_info.name, config.num_tests_enabled, tests_succeeded, config.num_tests_enabled, tests_failed);
}



void NVPlay_Run()
{
/* Make sure the GPU is supported */
	if (!current_device.device_info.hal->init_function)
	{
		Logging_Write(log_level_error, "This GPU is not yet supported :(\n");
		NVPlay_Shutdown(NVPLAY_EXIT_CODE_UNIMPLEMENTED_GPU);
	}

	if (!current_device.device_info.hal->init_function())
	{
		Logging_Write(log_level_error, "GPU initialisation failed!\n");
		NVPlay_Shutdown(NVPLAY_EXIT_CODE_NO_GPU_INIT);
	}	

	current_device.initialised = true; 

	// Make sure that nv_pmc_boot_0 got set
	current_device.nv_pmc_boot_0 = NV_ReadMMIO32(NV_PMC_BOOT);

	if (command_line.load_reg_script)
		NVPlay_RunScript();
	else if (command_line.use_test_ini)
		NVPlay_RunTests(); 
	else
		NVPlay_Repl();
}

void NVPlay_ShowHelpAndExit()
{
	printf("%s", msg_help); // Done this way to shut up GCC
	NVPlay_Shutdown(NVPLAY_EXIT_CODE_HELP_MENU);
}

/* Initialise NVPlay! */
bool NVPlay_Init(int32_t argc, char** argv)
{
	_gdb_start(); // gdb_start but it doesn't actually break into the debugger automatically

	Cmdline_Parse(argc, argv);

	log_settings.destination = (log_dest_file | log_dest_console);
	log_settings.flush_on_line = true; //bad idea?
	log_settings.level = (log_level_debug | log_level_message | log_level_warning | log_level_error);
	log_settings.valid = true;
	log_settings.redirect = log_redirect_stdin;
	
	if (!Logging_Init())
	{
		printf("Failed to initialise logging system\n");
		NVPlay_Shutdown(NVPLAY_EXIT_CODE_LOG_INIT_FAIL);
	}

	Logging_Write(log_level_message, APP_SIGNON_STRING);

	// early return
	if (command_line.show_help)
	{
		NVPlay_ShowHelpAndExit();
		return true;
	}

	NVPlay_DetectWindows();

	if (!PCI_BiosIsPresent())
		NVPlay_Shutdown(NVPLAY_EXIT_CODE_NO_PCI);

	if (!GPU_Detect())
		NVPlay_Shutdown(NVPLAY_EXIT_CODE_UNSUPPORTED_GPU);

	if (!Config_Load())
		NVPlay_Shutdown(NVPLAY_EXIT_CODE_CONFIG_LOAD_FAIL); 

	return true; 
}

void NVPlay_Shutdown(uint32_t exit_code)
{
	if (current_device.initialised 
		&& current_device.device_info.hal->shutdown_function)
		current_device.device_info.hal->shutdown_function();

	Logging_Shutdown();
	exit(exit_code);
}

int main(int argc, char** argv) 
{
	// Normally, initialisation errors would be handled, but if they aren't let's exit
	if (!NVPlay_Init(argc, argv))
	{
		if (log_settings.open)
			Logging_Write(log_level_warning, "Unknown fatal initialisation error!");
		else
		 	printf("Unknown fatal initialisation error!\n");

		NVPlay_Shutdown(NVPLAY_EXIT_CODE_UNKNOWN_FATAL);
	}

	if (!command_line.boot_only)
	{
		NVPlay_Run();
		NVPlay_Shutdown(NVPLAY_EXIT_CODE_SUCCESS);
	}
	
 	return 0;
}