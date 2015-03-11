#ifndef __LIBDEB_PROPERTIES_H
#define __LIBDEB_PROPERTIES_H

#include <debian/libdeb/config.h>

#ifdef __cplusplus
extern "C" {
#endif

/* forward declaration of glib hash table */
struct _GHashTable;
typedef struct _GHashTable GHashTable;

LIBDEB_EXPORT GHashTable *   deb_property_list_parse(char *str);

#ifdef __cplusplus
}
#endif

#endif /* __LIBDEB_PROPERTIES_H */