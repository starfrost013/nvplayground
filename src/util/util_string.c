/* 
    NVPlayground
    Copyright © 2025 starfrost

    Raw GPU programming for early Nvidia GPUs
    Licensed under the MIT license (see license file)

    util_string.c: String utilities (Mostly stolen from S7e)
*/

#include "core/nvcore.h"
#include <nvplayground.h>
#include <string.h>

bool String_IsEntirelyWhitespace(char* fmt, uint32_t max)
{
    if (!max)
        max = MAX_STR;

    uint32_t current = 0;

    while (*fmt != '\0')
    {
        if (!isspace(*fmt))
            return false;

        fmt++;
        current++;

        if (current >= max)
            return true;
    }

    return true; 
}

// Left-trims a string.
char* String_LTrim(char* fmt, uint32_t max)
{
    if (!max)
        max = MAX_STR;

    uint32_t current = 0;

    while (isspace(*fmt))
    {
        fmt++;
        current++;

        if (current >= max)
            return NULL;
    }

    return fmt; 
}

// Right-trims a string
char* String_RTrim(char* fmt, uint32_t max)
{
    if (!max)
        max = MAX_STR;

    uint32_t length = strlen(fmt);

    if (length > max)
        return NULL;

    // don't shred the entire string if it doesn't start with a space
    if (!isspace(fmt[length]))
        return fmt;

    uint32_t current = length;

    fmt = &fmt[length];

    while (isspace(*fmt))
    {
        fmt[current] = '\0';

        fmt--;
        current--;

        if (current < 0)
            return STRING_EMPTY;
    }

    return fmt; 
}

char* String_GetTokenSeparatedPart(char* fmt, const char* delim, uint32_t n)
{
    /* sanity checks */

    if (!fmt ||  !delim)
        return NULL; 

    size_t fmt_len = strlen(fmt);

    if (fmt_len < 0)
        return NULL;

    char* tok = strtok(fmt, delim);

    for (uint32_t tok_id = 0; tok_id < n; tok_id++)
    {
        tok = strtok(NULL, delim);

        if (!tok)
            break; 
    }

    return tok;
}