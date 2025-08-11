/* 
    NVPlay
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    format_nvss.c: Implements the "NVSS" savestate format (.NVS extension)
*/

#include <stdint.h>
#include <stdio.h>


#include "util/util.h"
#include <nvplayground.h>
#include <config/config.h>

bool GPUS_Load()
{
    gpus_header_t header = {0}; 

    Logging_Write(log_level_message, "Opening GPUS file at %s\n", command_line.savestate_file);

    FILE* stream = fopen(command_line.savestate_file, "rb+");

    if (!stream)
    {
        Logging_Write(log_level_error, "Failed to open GPUS file!\n");
        return false;
    }

    fread(&header, sizeof(gpus_header_t), 1, stream);

    Logging_Write(log_level_debug, "GPUS Header Information:\n");
    Logging_Write(log_level_debug, "Magic: %08X\n", header.magic);
    Logging_Write(log_level_debug, "GPUS Version: %04X\n", header.version);
    Logging_Write(log_level_debug, "Number of Sections: %04X\n", header.num_sections);
    Logging_Write(log_level_debug, "Device ID: %04x\n", header.device_id);

    if (header.magic != GPUS_MAGIC)
    {
        Logging_Write(log_level_error, "GPUS Parser: Invalid GPUS magic. Not a GPUS file!\n");
        return false; 
    }

    if (header.version != GPUS_VERSION)
    {
        Logging_Write(log_level_error, "GPUS Parser: Invalid GPUS version!\n");
        return false; 
    }
    
    if (header.num_sections == 0
    || header.num_sections > GPUS_SECTIONS_MAX)
    {
        Logging_Write(log_level_error, "GPUS Parser: Ridiculous number of sections (%d)!\n", header.num_sections);
        return false;
    }

    if (!current_device.device_info.gpus_section_applies
        || !current_device.device_info.gpus_section_parse)
    {
        Logging_Write(log_level_error, "GPUS Parser: GPU %s does not have GPUS section parsing functions defined!\n", current_device.device_info.name);
        return false; 
    }

    gpus_header_section_t current_section = {0}; 

    uint32_t total_section_size = 0; 

    // read in sections
    // if the GPU-specific parser doesn't parse it implement the default parser
    for (uint32_t i = 0; i < header.num_sections; i++)
    {
        fread(&current_section, sizeof(gpus_header_section_t), 1, stream);

        Logging_Write(log_level_debug, "Reading section %04X offset=%08X size=%08X\n", current_section.fourcc, current_section.offset, current_section.size);
        total_section_size += current_section.size; 
    }

    fseek(stream, 0, SEEK_END);

    uint32_t real_size = ftell(stream) - (sizeof(gpus_header_t) + (sizeof(gpus_header_section_t) * header.num_sections) + total_section_size);

    // not really fool proof validation but i also dont care
    // if you are running this program you already know what you're doing
    if (real_size < total_section_size)
    {
        Logging_Write(log_level_error, "GPUS file truncated! (should be %d bytes, is %d!)\n", total_section_size, real_size);
        return false;
    }

    fseek(stream, sizeof(gpus_header_t), SEEK_SET);

    // reac each section
    for (uint32_t i = 0; i < header.num_sections; i++)
    {        
        fread(&current_section, sizeof(gpus_header_section_t), 1, stream);

        Logging_Write(log_level_debug, "Parsing section %04X offset=%08X size=%08X\n", current_section.fourcc, current_section.offset, current_section.size);
        
        if (current_device.device_info.gpus_section_applies(current_section.fourcc))
        {   
            current_device.device_info.gpus_section_parse(current_section.fourcc, stream);
        }   
        else
        { 
            // *TECHNICALLY* declaration after label e.g. case is valid c23 but i am not taking chances
            uint32_t num_crtc_registers = 0, num_gdc_registers = 0, num_seq_registers = 0, num_attr_registers = 0;

            switch (current_section.fourcc)
            {
                default:
                    Logging_Write(log_level_warning, "GPUS Parser: Section %04x was not implemented by either the standard or GPU-specific parser\n", current_section.fourcc);
                    break; 
                case gpus_section_vga_crtc:
                    // start reading off crtc registers
                    num_crtc_registers = fread(&current_section, sizeof(uint32_t), 1, stream);

                    for (uint32_t crtc_registers = 0; crtc_registers < num_crtc_registers; crtc_registers++)
                    {
                        uint8_t crtc_index = fread(&current_section, sizeof(uint8_t), 1, stream);
                        uint8_t crtc_value = fread(&current_section, sizeof(uint8_t), 1, stream);

                        vga_crtc_write(crtc_index, crtc_value);
                    }

                    break;
                case gpus_section_vga_gdc:
                    num_gdc_registers = fread(&current_section, sizeof(uint32_t), 1, stream);

                    for (uint32_t gdc_registers = 0; gdc_registers < num_gdc_registers; gdc_registers++)
                    {
                        uint8_t gdc_index = fread(&current_section, sizeof(uint8_t), 1, stream);
                        uint8_t gdc_value = fread(&current_section, sizeof(uint8_t), 1, stream);

                        vga_gdc_write(gdc_index, gdc_value);
                    }
                    break;
                case gpus_section_vga_sequencer:
                    num_seq_registers = fread(&current_section, sizeof(uint32_t), 1, stream);

                    for (uint32_t seq_registers = 0; seq_registers < num_seq_registers; seq_registers++)
                    {
                        uint8_t seq_index = fread(&current_section, sizeof(uint8_t), 1, stream);
                        uint8_t seq_value = fread(&current_section, sizeof(uint8_t), 1, stream);

                        vga_sequencer_write(seq_index, seq_value);
                    }
                    break;
                case gpus_section_vga_attribute:
                    num_attr_registers = fread(&current_section, sizeof(uint32_t), 1, stream);

                    for (uint32_t attr_registers = 0; attr_registers < num_attr_registers; attr_registers++)
                    {
                        uint8_t attr_index = fread(&current_section, sizeof(uint8_t), 1, stream);
                        uint8_t attr_value = fread(&current_section, sizeof(uint8_t), 1, stream);

                        vga_attribute_write(attr_index, attr_value);
                    }
                    break;
            }
        }

       
    }
    return true; 
}