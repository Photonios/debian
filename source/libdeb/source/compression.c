#include <debian/libdeb/compression.h>

static const char * __compression_name_lookup__[DEB_COMPRESSION_COUNT] = {
	"invalid",	/* DEB_COMPRESSION_INVALID */
	"none",		/* DEB_COMPRESSION_NONE */
	"gz",		/* DEB_COMPRESSION_GZIP */
	"bz2",		/* DEB_COMPRESSION_BZIP2 */
	"lzma",		/* DEB_COMPRESSION_LZMA */
	"xz",		/* DEB_COMPRESSION_XZ */
	"uu",		/* DEB_COMPRESSION_UU */
	"rpm",		/* DEB_COMPRESSION_RPM */
	"lz",		/* DEB_COMPRESSION_LZIP */
	"lrz"		/* DEB_COMPRESSION_LRZIP */
};

const char * deb_compression_get_name(DEB_COMPRESSION compression)
{
	return __compression_name_lookup__[compression];
}