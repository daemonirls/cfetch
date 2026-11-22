#ifndef CFETCH_INFO_H
#define CFETCH_INFO_H

#include <stddef.h>

#define INFO_MAX 256

struct system_info {
    char os[INFO_MAX];
    char kernel[INFO_MAX];
    char host[INFO_MAX];
};

int read_distro(char *buf, size_t len);
int get_kernel(char *buf, size_t len);
int get_hostname(char *buf, size_t len);
int collect_all(struct system_info *out);

#endif
