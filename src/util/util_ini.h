/* 
 * NVPlay
 * Copyright © 2025-2026 starfrost
 *
 * Raw GPU programming for early Nvidia GPUs
 * Licensed under the MIT license (see license file)
 *
 * This file was taken from 86Box, <https://github.com/86Box/86Box>
 *
 * Configuration file manager header
 *
 * Authors: Sarah Walker, <https://pcem-emulator.co.uk/>
 *          Miran Grca, <mgrca8@gmail.com>
 *          Fred N. van Kempen, <decwiz@yahoo.com>
 *          Overdoze,
 *          David Hrdlička, <hrdlickadavid@outlook.com>
 *          Connor Hyde, <mario64crashed@gmail.com, nomorestarfrost@gmail.com>
 *
 *          Copyright 2008-2019 Sarah Walker.
 *          Copyright 2016-2019 Miran Grca.
 *          Copyright 2017-2019 Fred N. van Kempen.
 *          Copyright 2018-2019 David Hrdlička.
 *          Copyright 2025-2026 Connor Hyde
 *
 * NOTE:
 *          This version has been cut down for use in nvplay 
 *          ANSI_CFG is always on. This was for Win32 XP/Vista and we don't use Win32
 *          86box-isms were removed: 
 *              - OS-specific stuff was removed.
 *              - #if 0'd float code was removed (it didn't seem very robust and I don't even use this anyway)
 *              - Logging was removed.
 *              - wchar_t stuff was removed, since we're running under DOS
 *              - ini_dump was removed due to dependence on 86Box logging subsystem
 *              - added ini_section_get_hex32 for NV purposes (all NV registers are 32bit)
 *              - use stdint everywhere
 */


#pragma once

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void *ini_t;
typedef void *ini_section_t;

extern ini_t ini_new(void);
ini_t ini_read(const char *fn);
extern void  ini_strip_quotes(ini_t ini);
extern void  ini_write(ini_t ini, const char *fn);
extern void  ini_close(ini_t ini);

extern void     ini_section_delete_var(ini_section_t section, const char *name);
extern int32_t  ini_section_get_int(ini_section_t section, const char *name, int32_t def);
extern uint32_t ini_section_get_uint(ini_section_t section, const char *name, uint32_t def);
extern double   ini_section_get_double(ini_section_t section, const char *name, double def);
extern int32_t  ini_section_get_hex16(ini_section_t section, const char *name, int32_t def);
extern int32_t  ini_section_get_hex20(ini_section_t section, const char *name, int32_t def);
extern int32_t  ini_section_get_hex32(ini_section_t section, const char *name, int32_t def);
extern int32_t  ini_section_get_mac(ini_section_t section, const char *name, int32_t def);
extern char    *ini_section_get_string(ini_section_t section, const char *name, char *def);
extern void     ini_section_set_int(ini_section_t section, const char *name, int32_t val);
extern void     ini_section_set_uint(ini_section_t section, const char *name, uint32_t val);
extern void     ini_section_set_double(ini_section_t section, const char *name, double val);
extern void     ini_section_set_hex16(ini_section_t section, const char *name, int32_t val);
extern void     ini_section_set_hex20(ini_section_t section, const char *name, int32_t val);
extern void     ini_section_set_hex32(ini_section_t section, const char *name, int32_t val);
extern void     ini_section_set_mac(ini_section_t section, const char *name, int32_t val);
extern void     ini_section_set_string(ini_section_t section, const char *name, const char *val);
extern int32_t  ini_has_entry(ini_section_t self, const char *name);

#define ini_delete_var(ini, head, name)       ini_section_delete_var(ini_find_section(ini, head), name)

#define ini_get_int(ini, head, name, def)     ini_section_get_int(ini_find_section(ini, head), name, def)
#define ini_get_uint(ini, head, name, def)    ini_section_get_uint(ini_find_section(ini, head), name, def)
#define ini_get_double(ini, head, name, def)  ini_section_get_double(ini_find_section(ini, head), name, def)
#define ini_get_hex16(ini, head, name, def)   ini_section_get_hex16(ini_find_section(ini, head), name, def)
#define ini_get_hex20(ini, head, name, def)   ini_section_get_hex20(ini_find_section(ini, head), name, def)
#define ini_get_mac(ini, head, name, def)     ini_section_get_mac(ini_find_section(ini, head), name, def)
#define ini_get_string(ini, head, name, def)  ini_section_get_string(ini_find_section(ini, head), name, def)

#define ini_set_int(ini, head, name, val)     ini_section_set_int(ini_find_or_create_section(ini, head), name, val)
#define ini_set_uint(ini, head, name, val)    ini_section_set_uint(ini_find_or_create_section(ini, head), name, val)
#define ini_set_double(ini, head, name, val)  ini_section_set_double(ini_find_or_create_section(ini, head), name, val)
#define ini_set_hex16(ini, head, name, val)   ini_section_set_hex16(ini_find_or_create_section(ini, head), name, val)
#define ini_set_hex20(ini, head, name, val)   ini_section_set_hex20(ini_find_or_create_section(ini, head), name, val)
#define ini_set_hex32(ini, head, name, val)   ini_section_set_hex32(ini_find_or_create_section(ini, head), name, val)

#define ini_set_mac(ini, head, name, val)     ini_section_set_mac(ini_find_or_create_section(ini, head), name, val)
#define ini_set_string(ini, head, name, val)  ini_section_set_string(ini_find_or_create_section(ini, head), name, val)
#define ini_set_wstring(ini, head, name, val) ini_section_set_wstring(ini_find_or_create_section(ini, head), name, val)

extern ini_section_t ini_find_section(ini_t ini, const char *name);
extern ini_section_t ini_find_or_create_section(ini_t ini, const char *name);
extern void          ini_rename_section(ini_section_t section, const char *name);
extern void          ini_delete_section_if_empty(ini_t ini, ini_section_t section);

#ifdef __cplusplus
}
#endif
