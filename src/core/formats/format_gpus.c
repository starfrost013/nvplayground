/* 
    NVPlayground
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    format_nvss.c: Implements the "NVSS" savestate format (.NVS extension)
*/

#include <stdio.h>


#include "util/util.h"
#include <nvplayground.h>
#include <config/config.h>

bool GPUS_Load(const char* filename)
{
    gpus_header_t header  = {0}; 

    FILE* stream = fopen(filename, "rb+");

    fread(&header, sizeof(gpus_header_t), 1, stream);

    if (header.magic != GPUS_MAGIC)
    {
        Logging_Write(log_level_error, "GPUS Parser: Invalid GPUS magic. Not a GPUS file\n");
        return false; 
    }

    if (header.version != GPUS_VERSION)
    {
        Logging_Write(log_level_error, "GPUS Parser: Invalid GPUS version\n");
        return false; 
    }
    
    if (header.num_sections == 0
    || header.num_sections > GPUS_SECTIONS_MAX)
    {
        Logging_Write(log_level_error, "GPUS Parser: Ridiculous number of sections (%d)\n", header.num_sections);
        return false;
    }

    if (!current_device.device_info.gpus_section_applies
        || !current_device.device_info.gpus_section_parse)
    {
        Logging_Write(log_level_error, "GPUS Parser: GPU %s does not have GPUS section parsing functions defined\n", current_device.device_info.name);
    }

    gpus_header_section_t current_section; 

    // read in sections
    // if the GPU-specific parser doesn't parse it implement the default parser
    for (uint32_t i = 0; i < header.num_sections; i++)
    {
        fread(&current_section, sizeof(gpus_header_section_t), 1, stream);

        if (current_device.device_info.gpus_section_applies(current_section.fourcc))
        {   
            current_device.device_info.gpus_section_parse();
        }   

        switch (current_section.fourcc)
        {
            default:
                Logging_Write(log_level_warning, "GPUS Parser: Section %04x was not implemented by either the standard or GPU-specific parser\n", current_section.fourcc);
                break; 
        }
    }
}