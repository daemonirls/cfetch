#define _DEFAULT_SOURCE 1

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "info.h"
#include "ascii.h"

#define GAP 2

struct field {
    const char *label;
    const char *value;
};

static int term_width(void)
{
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
        return 0;

    return ws.ws_col;
}

static int art_width(void)
{
    int width = 0;

    for (int i = 0; i < ART_LINES; i++) {
        int len = (int)strlen(ART[i]);
        if (len > width)
            width = len;
    }

    return width;
}

static int max_field_width(const struct field *fields, int count)
{
    int width = 0;

    for (int i = 0; i < count; i++) {
        int len = (int)strlen(fields[i].label) + 2 + (int)strlen(fields[i].value);
        if (len > width)
            width = len;
    }

    return width;
}

static void print_flat(const struct field *fields, int count)
{
    for (int i = 0; i < count; i++)
        printf("%s: %s\n", fields[i].label, fields[i].value);
}

static void print_two_columns(const struct field *fields, int count)
{
    int width = art_width();
    int lines = count > ART_LINES ? count : ART_LINES;

    for (int i = 0; i < lines; i++) {
        if (i < ART_LINES)
            printf("%-*s", width, ART[i]);
        else
            printf("%*s", width, "");

        if (i < count)
            printf("%*s%s: %s", GAP, "", fields[i].label, fields[i].value);

        putchar('\n');
    }
}

int main(void)
{
    struct system_info info;

    if (collect_all(&info) != 0) {
        fprintf(stderr, "cfetch: failed to collect any system info\n");
        return 1;
    }

    struct field all[] = {
        { "OS",     info.os     },
        { "Kernel", info.kernel },
        { "Host",   info.host   },
    };
    const int total = (int)(sizeof all / sizeof all[0]);

    struct field shown[sizeof all / sizeof all[0]];
    int count = 0;

    for (int i = 0; i < total; i++) {
        if (all[i].value[0] != '\0')
            shown[count++] = all[i];
    }

    if (count == 0) {
        fprintf(stderr, "cfetch: no fields to show\n");
        return 1;
    }

    int need = art_width() + GAP + max_field_width(shown, count);
    int tw = term_width();

    if (isatty(STDOUT_FILENO) && (tw == 0 || tw >= need))
        print_two_columns(shown, count);
    else
        print_flat(shown, count);

    return 0;
}
