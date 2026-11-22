#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>

#include "info.h"

static const char os_release[] = "/etc/os-release";

int read_distro(char *buf, size_t len)
{
    if (buf == NULL || len == 0)
        return -1;

    buf[0] = '\0';

    FILE *fp = fopen(os_release, "r");
    if (fp == NULL)
        return -1;

    char line[INFO_MAX];
    int found = 0;

    while (fgets(line, sizeof line, fp) != NULL) {
        if (strncmp(line, "PRETTY_NAME=", 12) != 0)
            continue;

        char *p = line + 12;

        if (*p == '"')
            p++;

        char *end = strpbrk(p, "\"\n");
        if (end)
            *end = '\0';

        snprintf(buf, len, "%s", p);
        found = 1;
        break;
    }

    fclose(fp);

    return found ? 0 : -1;
}

int get_kernel(char *buf, size_t len)
{
    if (buf == NULL || len == 0)
        return -1;

    buf[0] = '\0';

    struct utsname kernel_info;
    if (uname(&kernel_info) == -1)
        return -1;

    snprintf(buf, len, "%s %s", kernel_info.sysname, kernel_info.release);

    return 0;
}

int get_hostname(char *buf, size_t len)
{
    if (buf == NULL || len == 0)
        return -1;

    buf[0] = '\0';

    if (gethostname(buf, len) != 0) {
        buf[0] = '\0';
        return -1;
    }

    buf[len - 1] = '\0';

    return 0;
}

int collect_all(struct system_info *out)
{
    if (out == NULL)
        return -1;

    memset(out, 0, sizeof *out);

    int ok = 0;

    if (read_distro(out->os, sizeof out->os) == 0)
        ok++;
    if (get_kernel(out->kernel, sizeof out->kernel) == 0)
        ok++;
    if (get_hostname(out->host, sizeof out->host) == 0)
        ok++;

    return ok > 0 ? 0 : -1;
}
