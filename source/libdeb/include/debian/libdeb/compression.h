#ifndef __LIBDEB_COMPRESSION_H
#define __LIBDEB_COMPRESSION_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	DEB_COMPRESSION_INVALID,
	DEB_COMPRESSION_NONE,
	DEB_COMPRESSION_GZIP,
	DEB_COMPRESSION_LMZA
} DEB_COMPRESSION;

#ifdef __cplusplus
}
#endif

#endif /* __LIBDEB_COMPRESSION_H */