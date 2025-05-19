

#include "config/config.h"
#include <nvplayground.h>
#include <core/nvcore.h>
#include <config/config.h>

#define GDB_IMPLEMENTATION
#include "gdbstub.h"

void set_video_mode(int mode) {
  __dpmi_regs regs = {0};
  regs.x.ax = mode;
  __dpmi_int(0x10, &regs);
}

int main(void) 
{
	_gdb_start(); // gdb_start but it doesn't actually break into the debugger automatically

	printf(APP_SIGNON_STRING);

	if (!pci_bios_is_present())
		exit(1);

	if (!nv_detect())
		exit(2);

	Config_Load(); 

	/* Make sure the GPU is supported */
	if (!current_device.device_info.init_function)
	{
		printf("This GPU is not yet supported :(");
		exit(3);
	}

	if (!current_device.device_info.init_function())
		exit(4);
	
	/* If main_function is set, call it, otherwise run the tests from the INI */
	if (current_device.device_info.main_function)
	{
		printf("Main function mode\n");

		current_device.device_info.main_function();
	}
	else 
	{
		printf("GPU test mode\n");

		if (config.num_tests_enabled == 0)
		{
			printf("No tests to run. Exiting...\n");
			exit(5);
		}

		printf("Running %ld tests...\n", config.num_tests_enabled);

		/* run each loaded test in order */
		nv_config_test_entry_t* current_entry = config.test_list_head; 

		uint32_t tests_succeeded = 0, tests_failed = 0;

		while (current_entry)
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
					printf("Test %s succeeded\n", current_entry->name);

				}
				else
				{
					tests_failed++;
					printf("Test %s failed! :(\n", current_entry->name);

				}
			}
				
			current_entry = current_entry->next; 
		}

		printf("%s: %lu tests ran, %lu/%lu succeeded (%lu failed)", 
			current_device.device_info.name, config.num_tests_enabled, tests_succeeded, config.num_tests_enabled, tests_failed);

	}

  	//__djgpp_nearptr_enable(); for dos rom
	// kbhit() etc

  	gdb_checkpoint();

	if (current_device.device_info.shutdown_function)
		current_device.device_info.shutdown_function();
	
 	return 0;
}