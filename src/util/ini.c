/* 
 * NVPlayground © 2025 starfrost
 * This file was taken from 86Box, <https://github.com/86Box/86Box>
 *
 * Configuration file manager
 *
 * Authors: Sarah Walker, <https://pcem-emulator.co.uk/>
 *          Miran Grca, <mgrca8@gmail.com>
 *          Fred N. van Kempen, <decwiz@yahoo.com>
 *          Overdoze,
 *          David Hrdlička, <hrdlickadavid@outlook.com>
 *
 *          Copyright 2008-2019 Sarah Walker.
 *          Copyright 2016-2019 Miran Grca.
 *          Copyright 2017-2019 Fred N. van Kempen.
 *          Copyright 2018-2019 David Hrdlička.
 *          Copyright 2025      Connor Hyde
 *
 */

 
#include <util/ini.h>

typedef struct _list_ {
    struct _list_ *next;
} list_t;

typedef struct section_t {
    list_t list;

    char name[128];

    list_t entry_head;
} section_t;

typedef struct entry_t {
    list_t list;

    char    name[128];
    char    data[512];
} entry_t;

#define list_add(new, head)        \
    {                              \
        list_t *next = head;       \
                                   \
        while (next->next != NULL) \
            next = next->next;     \
                                   \
        (next)->next = new;        \
        (new)->next  = NULL;       \
    }

#define list_delete(old, head)          \
    {                                   \
        list_t *next = head;            \
                                        \
        while ((next)->next != old) {   \
            next = (next)->next;        \
        }                               \
                                        \
        (next)->next = (old)->next;     \
        if ((next) == (head))           \
            (head)->next = (old)->next; \
    }

#define min(a,b) \
({ __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
  _a < _b ? _a : _b; })

static section_t *
find_section(list_t *head, const char *name)
{
    section_t *sec     = (section_t *) head->next;
    const char blank[] = "";

    if (name == NULL)
        name = blank;

    while (sec != NULL) {
        if (!strncmp(sec->name, name, sizeof(sec->name)))
            return sec;

        sec = (section_t *) sec->list.next;
    }

    return NULL;
}

ini_section_t
ini_find_section(ini_t ini, const char *name)
{
    if (ini == NULL)
        return NULL;

    return (ini_section_t) find_section((list_t *) ini, name);
}

void
ini_rename_section(ini_section_t section, const char *name)
{
    section_t *sec = (section_t *) section;

    if (sec == NULL)
        return;

    memset(sec->name, 0x00, sizeof(sec->name));
    memcpy(sec->name, name, min(128, strlen(name) + 1));
}

static entry_t *
find_entry(section_t *section, const char *name)
{
    entry_t *ent;

    ent = (entry_t *) section->entry_head.next;

    while (ent != NULL) {
        if (!strncmp(ent->name, name, sizeof(ent->name)))
            return ent;

        ent = (entry_t *) ent->list.next;
    }

    return (NULL);
}

int32_t
ini_has_entry(ini_section_t self, const char *name)
{
    section_t     *section = (section_t *) self;
    const entry_t *entry;

    if (section == NULL)
        return 0;

    entry = find_entry(section, name);
    if (entry == NULL)
        return 0;
    
    return 1;
}

static int32_t
entries_num(section_t *section)
{
    entry_t *ent;
    int32_t      i = 0;

    ent = (entry_t *) section->entry_head.next;

    while (ent != NULL) {
        if (strlen(ent->name) > 0)
            i++;

        ent = (entry_t *) ent->list.next;
    }

    return i;
}

static void
delete_section_if_empty(list_t *head, section_t *section)
{
    if (section == NULL)
        return;

    int32_t     n = entries_num(section);

    if (n > 0) {
        int32_t      i      = 0;
        entry_t *i_ent = (entry_t *) section->entry_head.next;

        while (i_ent != NULL) {
            int32_t      i_nlen = strlen(i_ent->name);
            entry_t* i_next = (entry_t *) i_ent->list.next;

            if (i_nlen > 0) {
                int32_t      j      = 0;
                entry_t *j_ent = (entry_t *) section->entry_head.next;

                while (j_ent != NULL) {
                    int32_t      j_nlen = strlen(j_ent->name);
                    entry_t* j_next = (entry_t *) j_ent->list.next;
                    if (j_nlen > 0) {
                        if ((j != i) && (strcmp(j_ent->name, i_ent->name) > 0)) {
                            entry_t t_ent = { 0 };
                            memcpy(&t_ent, j_ent, sizeof(entry_t));
                            /* J: Contents of I, list of J */
                            memcpy(j_ent->name, i_ent->name, sizeof(entry_t) - sizeof(i_ent->list));
                            /* I: Contents of J, list of I */
                            memcpy(i_ent->name, t_ent.name, sizeof(entry_t) - sizeof(i_ent->list));
                        }

                        j++;
                    }

                    j_ent = (entry_t *) j_next;
                }

                i++;
            }

            i_ent = (entry_t *) i_next;
        }
    } else {
        list_delete(&section->list, head);
        free(section);
    }
}

void
ini_delete_section_if_empty(ini_t ini, ini_section_t section)
{
    if (ini == NULL || section == NULL)
        return;

    delete_section_if_empty((list_t *) ini, (section_t *) section);
}

static section_t *
create_section(list_t *head, const char *name)
{
    section_t *ns = calloc(1, sizeof(section_t));

    memcpy(ns->name, name, strlen(name) + 1);
    list_add(&ns->list, head);

    return ns;
}

ini_section_t
ini_find_or_create_section(ini_t ini, const char *name)
{
    if (ini == NULL)
        return NULL;

    section_t *section = find_section((list_t *) ini, name);
    if (section == NULL)
        section = create_section((list_t *) ini, name);

    return (ini_section_t) section;
}

static entry_t *
create_entry(section_t *section, const char *name)
{
    entry_t *ne = calloc(1, sizeof(entry_t));

    memcpy(ne->name, name, strlen(name) + 1);
    list_add(&ne->list, &section->entry_head);

    return ne;
}

void
ini_close(ini_t ini)
{
    section_t *sec;
    section_t *ns;
    entry_t   *ent;
    list_t    *list = (list_t *) ini;

    if (list == NULL)
        return;

    sec = (section_t *) list->next;
    while (sec != NULL) {
        ns  = (section_t *) sec->list.next;
        ent = (entry_t *) sec->entry_head.next;

        while (ent != NULL) {
            entry_t *nent = (entry_t *) ent->list.next;

            free(ent);
            ent = nent;
        }

        free(sec);
        sec = ns;
    }

    free(list);
}

static int
ini_detect_bom(const char *fn)
{
    FILE         *fp;
    unsigned char bom[4] = { 0, 0, 0, 0 };

    fp = fopen(fn, "rt");

    if (fp == NULL)
        return 0;
    (void) !fread(bom, 1, 3, fp);
    if (bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF) {
        fclose(fp);
        return 1;
    }
    fclose(fp);
    return 0;
}

/* Read and parse the configuration file into memory. */
ini_t
ini_read(const char *fn)
{
    char       sname[128];
    char       ename[128];
    char       buff[1024];
    section_t *sec;
    section_t *ns;
    entry_t   *ne;
    int32_t        c;
    int32_t        d;
    int32_t        bom;
    FILE      *fp;
    list_t    *head;

    bom = ini_detect_bom(fn);

    fp = fopen(fn, "rt");

    if (fp == NULL)
        return NULL;

    head = calloc(1, sizeof(list_t));
    sec = calloc(1, sizeof(section_t));

    list_add(&sec->list, head);
    if (bom)
        fseek(fp, 3, SEEK_SET);

    while (1) {
        memset(buff, 0x00, sizeof(buff));

        (void) !fgets(buff, sizeof(buff), fp);

        if (feof(fp))
            break;

        /* Make sure there are no stray newlines or hard-returns in there. */
        if (strlen(buff) > 0)
            if (buff[strlen(buff) - 1] == '\n')
                buff[strlen(buff) - 1] = '\0';
        if (strlen(buff) > 0)
            if (buff[strlen(buff) - 1] == '\r')
                buff[strlen(buff) - 1] = '\0';

        /* Skip any leading whitespace. */
        c = 0;
        while ((buff[c] == ' ') || (buff[c] == '\t'))
            c++;

        /* Skip empty lines. */
        if (buff[c] == '\0')
            continue;

        /* Skip lines that (only) have a comment. */
        if ((buff[c] == '#') || (buff[c] == ';'))
            continue;

        if (buff[c] == '[') { /*Section*/
            c++;
            d = 0;
            while (buff[c] != ']' && buff[c])
                sname[d++] = buff[c++];

            sname[d] = L'\0';

            /* Is the section name properly terminated? */
            if (buff[c] != ']')
                continue;

            /* Create a new section and insert it. */
            ns = malloc(sizeof(section_t));
            memset(ns, 0x00, sizeof(section_t));
            memcpy(ns->name, sname, 128);
            list_add(&ns->list, head);

            /* New section is now the current one. */
            sec = ns;
            continue;
        }

        /* Get the variable name. */
        d = 0;
        while ((buff[c] != '=') && (buff[c] != ' ') && buff[c])
            ename[d++] = buff[c++];

        ename[d] = L'\0';

        /* Skip incomplete lines. */
        if (buff[c] == L'\0')
            continue;

        /* Look for =, skip whitespace. */
        while ((buff[c] == '=' || buff[c] == ' ') && buff[c])
            c++;

        /* Skip incomplete lines. */
        if (buff[c] == L'\0')
            continue;

        /* This is where the value part starts. */
        d = c;

        /* Allocate a new variable entry.. */
        ne = calloc(1, sizeof(entry_t));
        memset(ne, 0x00, sizeof(entry_t));
        memcpy(ne->name, ename, 128);
        strncpy(ne->data, &buff[d], sizeof(ne->data) - 1);

        ne->data[sizeof(ne->data) - 1] = '\0';

        /* .. and insert it. */
        list_add(&ne->list, &sec->entry_head);
    }

    (void) fclose(fp);

    return (ini_t) head;
}

/* Write the in-memory configuration to disk. */
void
ini_write(ini_t ini, const char *fn)
{
    char      temp[512];
    list_t    *list = (list_t *) ini;
    section_t *sec;
    FILE      *fp;
    int32_t        fl = 0;

    if (list == NULL)
        return;

    sec = (section_t *) list->next;

    fp = fopen(fn, "wt");

    if (fp == NULL)
        return;

    while (sec != NULL) {
        entry_t *ent;

        if (sec->name[0]) {
            if (fl)
                fprintf(fp, "\n[%s]\n", temp);
            else
                fprintf(fp, "[%s]\n", temp);
            fl++;
        }

        ent = (entry_t *) sec->entry_head.next;
        while (ent != NULL) {
            if (ent->name[0] != '\0') {
                if (ent->data[0] == '\0')
                    fprintf(fp, "%s = \n", temp);
                else
                    fprintf(fp, "%s = %s\n", temp, ent->data);
                fl++;
            }

            ent = (entry_t *) ent->list.next;
        }

        sec = (section_t *) sec->list.next;
    }

    (void) fclose(fp);
}

// https://stackoverflow.com/a/122974
// taken from 86box net_modem.c
char *
trim(char *str)
{
    size_t len    = 0;
    char  *frontp = str;
    char  *endp   = NULL;

    if (str == NULL) {
        return NULL;
    }
    if (str[0] == '\0') {
        return str;
    }

    len  = strlen(str);
    endp = str + len;

    /* Move the front and back pointers to address the first non-whitespace
     * characters from each end.
     */
    while (isspace((unsigned char) *frontp)) {
        ++frontp;
    }
    if (endp != frontp) {
        while (isspace((unsigned char) *(--endp)) && endp != frontp) { }
    }

    if (frontp != str && endp == frontp)
        *str = '\0';
    else if (str + len - 1 != endp)
        *(endp + 1) = '\0';

    /* Shift the string so that it starts at str so that if it's dynamically
     * allocated, we can still free it on the returned pointer.  Note the reuse
     * of endp to mean the front of the string buffer now.
     */
    endp = str;
    if (frontp != str) {
        while (*frontp) {
            *endp++ = *frontp++;
        }
        *endp = '\0';
    }

    return str;
}

void
ini_strip_quotes(ini_t ini)
{
    list_t    *list = (list_t *) ini;
    section_t *sec;

    sec = (section_t *) list->next;

    while (sec != NULL) {
        entry_t *ent;

        ent = (entry_t *) sec->entry_head.next;
        while (ent != NULL) {
            if (ent->name[0] != '\0') {
                int32_t trailing_hash = strcspn(ent->data, "#");
                int32_t trailing_quote;
                ent->data[trailing_hash] = 0;

                if (ent->data[0] == '\"') {
                    memmove(ent->data, &ent->data[1], sizeof(ent->data) - sizeof(char));
                }
                if (ent->data[strlen(ent->data) - 1] == '\"') {
                    ent->data[strlen(ent->data) - 1] = 0;
                }

                trailing_quote = strcspn(ent->data, "\"");
                ent->data[trailing_quote] = 0;
                
                trim(ent->data);
            }

            ent = (entry_t *) ent->list.next;
        }

        sec = (section_t *) sec->list.next;
    }
}

ini_t
ini_new(void)
{
    ini_t ini = calloc(1, (sizeof(list_t)));
    return ini;
}

void
ini_section_delete_var(ini_section_t self, const char *name)
{
    section_t *section = (section_t *) self;
    entry_t   *entry;

    if (section == NULL)
        return;

    entry = find_entry(section, name);
    if (entry != NULL) {
        list_delete(&entry->list, &section->entry_head);
        free(entry);
    }
}

int32_t
ini_section_get_int(ini_section_t self, const char *name, int32_t def)
{
    section_t     *section = (section_t *) self;
    const entry_t *entry;
    int32_t            value = 0;

    if (section == NULL)
        return def;

    entry = find_entry(section, name);
    if (entry == NULL)
        return def;

    if (stricmp(entry->data, "true") == 0)
        return 1;
    if (stricmp(entry->data, "false") == 0)
        return 0;

    sscanf(entry->data, "%li", &value);

    return value;
}

uint32_t
ini_section_get_uint(ini_section_t self, const char *name, uint32_t def)
{
    section_t     *section = (section_t *) self;
    const entry_t *entry;
    uint32_t       value = 0;

    if (section == NULL)
        return def;

    entry = find_entry(section, name);
    if (entry == NULL)
        return def;

    sscanf(entry->data, "%lu", &value);

    return value;
}


double
ini_section_get_double(ini_section_t self, const char *name, double def)
{
    section_t   *section = (section_t *) self;
    entry_t     *entry;
    double      value = 0;
    int32_t     res = 0;

    if (section == NULL)
        return def;

    entry = find_entry(section, name);
    if (entry == NULL)
        return def;

    res = sscanf(entry->data, "%lg", &value);
    if (res == EOF || res <= 0) {
        int32_t i = 0;
        for (i = 0; i < strlen(entry->data); i++) {
            if (entry->data[i] == ',') {
                entry->data[i] = '.';
            }
        }
        (void)sscanf(entry->data, "%lg", &value);
    }

    return value;
}

int32_t
ini_section_get_hex16(ini_section_t self, const char *name, int32_t def)
{
    section_t     *section = (section_t *) self;
    const entry_t *entry;
    uint32_t   value = 0;

    if (section == NULL)
        return def;

    entry = find_entry(section, name);
    if (entry == NULL)
        return def;

    sscanf(entry->data, "%04lX", &value);

    return value;
}

int32_t
ini_section_get_hex20(ini_section_t self, const char *name, int32_t def)
{
    section_t     *section = (section_t *) self;
    const entry_t *entry;
    uint32_t   value = 0;

    if (section == NULL)
        return def;

    entry = find_entry(section, name);
    if (entry == NULL)
        return def;

    sscanf(entry->data, "%05lX", &value);

    return value;
}

int32_t
ini_section_get_hex32(ini_section_t self, const char *name, int32_t def)
{
    section_t     *section = (section_t *) self;
    const entry_t *entry;
    uint32_t   value = 0;

    if (section == NULL)
        return def;

    entry = find_entry(section, name);
    if (entry == NULL)
        return def;

    sscanf(entry->data, "%08lX", &value);

    return value;
}

int32_t
ini_section_get_mac(ini_section_t self, const char *name, int32_t def)
{
    section_t     *section = (section_t *) self;
    const entry_t *entry;
    uint32_t   val0 = 0;
    uint32_t   val1 = 0;
    uint32_t   val2 = 0;

    if (section == NULL)
        return def;

    entry = find_entry(section, name);
    if (entry == NULL)
        return def;

    sscanf(entry->data, "%02lx:%02lx:%02lx", &val0, &val1, &val2);

    return ((val0 << 16) + (val1 << 8) + val2);
}

char *
ini_section_get_string(ini_section_t self, const char *name, char *def)
{
    section_t *section = (section_t *) self;
    entry_t   *entry;

    if (section == NULL)
        return def;

    entry = find_entry(section, name);
    if (entry == NULL)
        return def;

    return (entry->data);
}

void
ini_section_set_int(ini_section_t self, const char *name, int32_t val)
{
    section_t *section = (section_t *) self;
    entry_t   *ent;

    if (section == NULL)
        return;

    ent = find_entry(section, name);
    if (ent == NULL)
        ent = create_entry(section, name);

    sprintf(ent->data, "%li", val);
}

void
ini_section_set_uint(ini_section_t self, const char *name, uint32_t val)
{
    section_t *section = (section_t *) self;
    entry_t   *ent;

    if (section == NULL)
        return;

    ent = find_entry(section, name);
    if (ent == NULL)
        ent = create_entry(section, name);

    sprintf(ent->data, "%li", val);
}

void
ini_section_set_double(ini_section_t self, const char *name, double val)
{
    section_t *section = (section_t *) self;
    entry_t   *ent;

    if (section == NULL)
        return;

    ent = find_entry(section, name);
    if (ent == NULL)
        ent = create_entry(section, name);

    sprintf(ent->data, "%lg", val);
}

void
ini_section_set_hex16(ini_section_t self, const char *name, int32_t val)
{
    section_t *section = (section_t *) self;
    entry_t   *ent;

    if (section == NULL)
        return;

    ent = find_entry(section, name);
    if (ent == NULL)
        ent = create_entry(section, name);

    sprintf(ent->data, "%04lX", val);
}

void
ini_section_set_hex20(ini_section_t self, const char *name, int32_t val)
{
    section_t *section = (section_t *) self;
    entry_t   *ent;

    if (section == NULL)
        return;

    ent = find_entry(section, name);
    if (ent == NULL)
        ent = create_entry(section, name);

    sprintf(ent->data, "%05lX", val);
}

void
ini_section_set_hex32(ini_section_t self, const char *name, int32_t val)
{
    section_t *section = (section_t *) self;
    entry_t   *ent;

    if (section == NULL)
        return;

    ent = find_entry(section, name);
    if (ent == NULL)
        ent = create_entry(section, name);

    sprintf(ent->data, "%08lX", val);
}

void
ini_section_set_mac(ini_section_t self, const char *name, int32_t val)
{
    section_t *section = (section_t *) self;
    entry_t   *ent;

    if (section == NULL)
        return;

    ent = find_entry(section, name);
    if (ent == NULL)
        ent = create_entry(section, name);

    sprintf(ent->data, "%02lx:%02lx:%02lx",
            (val >> 16) & 0xff, (val >> 8) & 0xff, val & 0xff);
}

void
ini_section_set_string(ini_section_t self, const char *name, const char *val)
{
    section_t *section = (section_t *) self;
    entry_t   *ent;

    if (section == NULL)
        return;

    ent = find_entry(section, name);
    if (ent == NULL)
        ent = create_entry(section, name);

    if ((strlen(val) + 1) <= sizeof(ent->data))
        memcpy(ent->data, val, strlen(val) + 1);
    else
        memcpy(ent->data, val, sizeof(ent->data));
}