#ifndef __LIBDEB_UTIL_H
#define __LIBDEB_UTIL_H

#include <debian/libdeb/config.h>

#ifdef __cplusplus
extern "C" {
#endif

void    strtolower(char *str);
int     strstartswith(const char *str, const char *substr);

#ifdef __cplusplus
}
#endif

#endif /* __LIBDEB_UTIL_H */