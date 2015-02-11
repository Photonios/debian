#include <stdio.h>

#include <debian/libdeb/file.h>

int main(int argc, char **argv)
{
	DEB_FILE *file = NULL;
	deb_file_open("C:\\Users\\Administrator\\Downloads\\google-chrome-stable_current_amd64.ar", &file);
	return 0;
}