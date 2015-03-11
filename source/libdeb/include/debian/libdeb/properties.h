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

#define DEB_PROPERTY_PACKAGE        "package"
#define DEB_PROPERTY_VERSION        "version"
#define DEB_PROPERTY_ARCHITECTURE   "architecture"
#define DEB_PROPERTY_MAINTAINER     "maintainer"
#define DEB_PROPERTY_INSTALLED_SIZE "installed-size"
#define DEB_PROPERTY_PRE_DEPENDS    "pre-depends"
#define DEB_PROPERTY_DEPENDS        "depends"
#define DEB_PROPERTY_PROVIDES       "provides"
#define DEB_PROPERTY_SECTION        "section"
#define DEB_PROPERTY_PRIORITY       "priority"
#define DEB_PROPERTY_DESCRIPTION    "description"

LIBDEB_EXPORT DEB_PROPERTIES *   deb_properties_parse(char *str);
LIBDEB_EXPORT void               deb_properties_print(DEB_PROPERTIES *properties);
LIBDEB_EXPORT char *             deb_properties_get(DEB_PROPERTIES *properties, const char *name);

#ifdef __cplusplus
}
#endif

#endif /* __LIBDEB_PROPERTIES_H */