#ifndef __LIBDEB_ARCHFORMAT_H
#define __LIBDEB_ARCHFORMAT_H

#include <debian/libdeb/config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DEB_ARCHFORMAT_INVALID              = 0,
	DEB_ARCHFORMAT_BASE_MASK            = 0xff0000,
    DEB_ARCHFORMAT_CPIO	                = 0x10000,
    DEB_ARCHFORMAT_CPIO_POSIX           = (DEB_ARCHFORMAT_CPIO | 1),
    DEB_ARCHFORMAT_CPIO_BIN_LE          = (DEB_ARCHFORMAT_CPIO | 2),
    DEB_ARCHFORMAT_CPIO_BIN_BE          = (DEB_ARCHFORMAT_CPIO | 3),
    DEB_ARCHFORMAT_CPIO_SVR4_NOCRC      = (DEB_ARCHFORMAT_CPIO | 4),
    DEB_ARCHFORMAT_CPIO_SVR4_CRC        = (DEB_ARCHFORMAT_CPIO | 5),
    DEB_ARCHFORMAT_CPIO_AFIO_LARGE      = (DEB_ARCHFORMAT_CPIO | 6),
    DEB_ARCHFORMAT_SHAR	                = 0x20000,
    DEB_ARCHFORMAT_SHAR_BASE            = (DEB_ARCHFORMAT_SHAR | 1),
    DEB_ARCHFORMAT_SHAR_DUMP            = (DEB_ARCHFORMAT_SHAR | 2),
    DEB_ARCHFORMAT_TAR                  = 0x30000,
    DEB_ARCHFORMAT_TAR_USTAR            = (DEB_ARCHFORMAT_TAR | 1),
    DEB_ARCHFORMAT_TAR_PAX_INTERCHANGE  = (DEB_ARCHFORMAT_TAR | 2),
    DEB_ARCHFORMAT_TAR_PAX_RESTRICTED   = (DEB_ARCHFORMAT_TAR | 3),
    DEB_ARCHFORMAT_TAR_GNUTAR           = (DEB_ARCHFORMAT_TAR | 4),
    DEB_ARCHFORMAT_ISO9660	            = 0x40000,
    DEB_ARCHFORMAT_ISO9660_ROCKRIDGE    = (DEB_ARCHFORMAT_ISO9660 | 1),
    DEB_ARCHFORMAT_ZIP                  = 0x50000,
    DEB_ARCHFORMAT_EMPTY                = 0x60000,
    DEB_ARCHFORMAT_AR                   = 0x70000,
    DEB_ARCHFORMAT_AR_GNU               = (DEB_ARCHFORMAT_AR | 1),
    DEB_ARCHFORMAT_AR_BSD               = (DEB_ARCHFORMAT_AR | 2),
    DEB_ARCHFORMAT_MTREE                = 0x80000,
    DEB_ARCHFORMAT_RAW                  = 0x90000,
    DEB_ARCHFORMAT_XAR                  = 0xA0000,
    DEB_ARCHFORMAT_LHA                  = 0xB0000,
    DEB_ARCHFORMAT_CAB                  = 0xC0000,
    DEB_ARCHFORMAT_RAR                  = 0xD0000,
    DEB_ARCHFORMAT_7ZIP                 = 0xE0000,
    DEB_ARCHFORMAT_WARC	                = 0xF0000
} DEB_ARCHFORMAT;

LIBDEB_EXPORT const char * deb_archformat_get_name(DEB_ARCHFORMAT format);

#ifdef __cplusplus
}
#endif

#endif /* __LIBDEB_ARCHFORMAT_H */