#include <ctype.h>
#include <string.h>

#include <debian/libdeb/util.h>

void
strtolower(char *str)
{
    char *str_ptr = str;
    while(*str_ptr != '\0') {
        if(!isalpha(*str_ptr)) {
            ++str_ptr;
            continue;
        }

        *str_ptr = tolower(*str_ptr);
        ++str_ptr;
    }
}

int
strstartswith(const char *str, const char *substr)
{
	int substr_len = strlen(substr);
	int result = strncmp(str, substr, substr_len);
	return result == 0 ? 1 : 0;
}