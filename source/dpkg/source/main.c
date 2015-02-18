#include <stdio.h>

#include <debian/libdeb/file.h>

int main(int argc, char **argv)
{
	DEB_FILE *file = NULL;
	DEB_RESULT res = deb_file_open("C:\\Users\\Administrator\\Downloads\\google-chrome-stable_current_amd64.ar", &file);
	const char *henk = deb_compression_get_name(file->meta_compression);
	return 0;
}