#ifndef __LIBDEB_RESULT_H
#define __LIBDEB_RESULT_H

#include <debian/libdeb/config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	DEB_RESULT_OK,
	DEB_RESULT_NULL_PARAM,
	DEB_RESULT_INVALID_VERSION,
	DEB_RESULT_NO_MEM,
	DEB_RESULT_TOO_SMALL,
	DEB_RESULT_READ_FAIL,
	DEB_RESULT_ARCHIVE_FAIL,
	DEB_RESULT_META_ARCHIVE_CORRUPT,
    DEB_RESULT_CONTROL_FILE_READ_FAIL,
    DEB_RESULT_CONTROL_FILE_PARSE_FAIL
} DEB_RESULT;

#define deb_result_is_good(res) \
	(res == DEB_RESULT_OK)

#define deb_result_is_bad(res) \
	(res != DEB_RESULT_OK)

#ifdef __cplusplus
extern "C" }
#endif

#endif /*__LIBDEB_RESULT_H */