#ifndef __LIBDEB_PROPERTIES_H
#define __LIBDEB_PROPERTIES_H

#include <debian/libdeb/config.h>

#ifdef __cplusplus
extern "C" {
#endif

/* forward declaration of glib hash table */
struct _GHashTable;
typedef struct _GHashTable GHashTable;

/* in case we ever want to get rid of glib */
typedef GHashTable DEB_PROPERTIES;

LIBDEB_EXPORT DEB_PROPERTIES *   deb_properties_parse(char *str);
LIBDEB_EXPORT void               deb_properties_print(DEB_PROPERTIES *properties);

#ifdef __cplusplus
}
#endif

#endif /* __LIBDEB_PROPERTIES_H */