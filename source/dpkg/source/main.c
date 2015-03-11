#include <stdio.h>

#include <debian/libdeb/file.h>

int main(int argc, char **argv)
{
	DEB_FILE *file = NULL;
	DEB_RESULT res = deb_file_open("C:\\Users\\Administrator\\Downloads\\google-chrome-stable_current_amd64.deb", &file);

	const char *meta_compression = deb_compression_get_name(file->meta_compression);
    const char *meta_archformat = deb_archformat_get_name(file->meta_archformat);

    const char *data_compression = deb_compression_get_name(file->data_compression);
    const char *data_archformat = deb_archformat_get_name(file->data_archformat);

    char *pkg_name = deb_properties_get(file->properties, DEB_PROPERTY_PACKAGE);
    char *pkg_version = deb_properties_get(file->properties, DEB_PROPERTY_VERSION);

    printf("Package name: %s\n", pkg_name);
    printf("Package version: %s\n", pkg_version);

    printf("Data compression: %s\n", data_compression);
    printf("Data archive format: %s\n", data_archformat);

    printf("Meta-data compression: %s\n", meta_compression);
    printf("Meta-data archive format: %s\n", meta_archformat);

	return 0;
}