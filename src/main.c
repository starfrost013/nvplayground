

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

	Config_Load(); 

	printf(APP_SIGNON_STRING);

	if (!pci_bios_is_present())
		exit(1);

	if (!nv_detect())
		exit(2);

	/* Make sure the GPU is supported */
	if (!current_device.device_info.init_function)
	{
		printf("This GPU is not yet supported :(");
		exit(3);
	}

	if (!current_device.device_info.init_function())
		exit(4);
	
  	//__djgpp_nearptr_enable(); for dos rom
	// kbhit() etc

  	gdb_checkpoint();

 	return 0;
}