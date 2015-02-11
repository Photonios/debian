#ifndef LIBDEB_CONFIG_H
#define LIBDEB_CONFIG_H
 
#ifdef __cplusplus
extern "C" {
#endif

#if defined(LIBDEB_CONFIG_EXPORT) && !defined(LIBDEB_CONFIG_IMPORT)
	#define LIBDEB_EXPORT __declspec(dllexport)
#elif defined(LIBDEB_CONFIG_IMPORT)
	#define LIBDEB_EXPORT __declspec(dllimport)
#else
	#define LIBDEB_EXPORT
#endif

#define DEB_PACKAGE_FORMAT_VERSION_MAJOR 2
#define DEB_PACKAGE_FORMAT_VERSION_MINOR 0
#define DEB_PACKAGE_FORMAT_VERSION_PATCH 0
#define DEB_PACKAGE_FORMAT_VERSION_BUILD 0

#ifdef __cplusplus
}
#endif

#endif /* LIBDEB_CONFIG_H */