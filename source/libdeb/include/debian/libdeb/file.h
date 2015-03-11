#ifndef __LIBDEB_FILE_H
#define __LIBDEB_FILE_H

#include <debian/libdeb/config.h>
#include <debian/libdeb/compression.h>
#include <debian/libdeb/archformat.h>
#include <debian/libdeb/result.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	DEB_COMPRESSION data_compression;
	DEB_COMPRESSION meta_compression;
    DEB_ARCHFORMAT  data_archformat;
    DEB_ARCHFORMAT  meta_archformat;
} DEB_FILE;

LIBDEB_EXPORT DEB_FILE *		deb_file_new();
LIBDEB_EXPORT DEB_RESULT		deb_file_open(const char *filename, DEB_FILE **file);

LIBDEB_EXPORT DEB_RESULT		deb_file_set_data_compression(DEB_FILE *file, DEB_COMPRESSION compression);
LIBDEB_EXPORT DEB_COMPRESSION	deb_file_get_data_compression(DEB_FILE *file);

LIBDEB_EXPORT DEB_RESULT		deb_file_set_meta_compression(DEB_FILE *file, DEB_COMPRESSION compression);
LIBDEB_EXPORT DEB_COMPRESSION	deb_file_get_meta_compression(DEB_FILE *file);

LIBDEB_EXPORT DEB_RESULT        deb_file_set_data_archformat(DEB_FILE *file, DEB_ARCHFORMAT format);
LIBDEB_EXPORT DEB_ARCHFORMAT    deb_file_get_data_archformat(DEB_FILE *file);

LIBDEB_EXPORT DEB_RESULT        deb_file_set_meta_archformat(DEB_FILE *file, DEB_ARCHFORMAT format);
LIBDEB_EXPORT DEB_ARCHFORMAT    deb_file_get_meta_archformat(DEB_FILE *file);

#ifdef __cplusplus
}
#endif

#endif /* __LIBDEB_FILE_H */