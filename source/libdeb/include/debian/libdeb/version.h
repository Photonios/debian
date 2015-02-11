#ifndef __LIBDEB_VERSION_H
#define __LIBDEB_VERSION_H

#include <stdint.h>

#include <debian/libdeb/config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint32_t major;
	uint32_t minor;
	uint32_t patch;
	uint32_t build;
} DEB_VERSION;

typedef enum {
	DEB_VERSION_EQUAL,
	DEB_VERSION_EQUAL_OR_HIGHER,
	DEB_VERSION_EQUAL_OR_LOWER,
	DEB_VERSION_LOWER,
	DEB_VERSION_HIGHER
} DEB_VERSION_OPERATOR;

#define DEB_VERSION_SEGMENT_COUNT 4

LIBDEB_EXPORT DEB_VERSION deb_version_new();
LIBDEB_EXPORT DEB_VERSION deb_version_create(uint32_t major, uint32_t minor, uint32_t patch, uint32_t build);
LIBDEB_EXPORT DEB_VERSION deb_version_parse(const char *str);

LIBDEB_EXPORT int deb_version_compare(DEB_VERSION version_a, DEB_VERSION version_b, DEB_VERSION_OPERATOR oper);
LIBDEB_EXPORT int deb_version_is_equal(DEB_VERSION version_a, DEB_VERSION version_b);
LIBDEB_EXPORT int deb_version_is_equal_or_higher(DEB_VERSION version_a, DEB_VERSION version_b);
LIBDEB_EXPORT int deb_version_is_equal_or_lower(DEB_VERSION version_a, DEB_VERSION version_b);
LIBDEB_EXPORT int deb_version_is_higher(DEB_VERSION version_a, DEB_VERSION version_b);
LIBDEB_EXPORT int deb_version_is_lower(DEB_VERSION version_a, DEB_VERSION version_b);

#ifdef __cplusplus
}
#endif

#endif /* __LIBDEB_VERSION_H */