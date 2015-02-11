#include <math.h>
#include <string.h>
#include <stdarg.h>

#include <debian/libdeb/version.h>

static uint32_t get_number(const char *str, int len)
{
	uint32_t result = 0;
	
	/* determine the amount of numbers in the string */
	int num_amount = 0;
	for(int i = 0; i < len; i++) {
		char c = str[i];
	
		if(c < '0' || c > '9') {
			continue;
		}
		
		num_amount++;
	}

	/* calculate actual number, skipping non-numbers */
	int num_counter = 0;
	for(int i = 0; i < len; i++) {
		char c = str[i];
		
		if(c < '0' || c > '9') {
			continue;
		}
		
		int number = (str[i] - '0');	
		int power = (int) pow(10, num_amount - num_counter - 1);
		int addition = number * power;
		
		if(addition >= 0)
			result += addition;

		num_counter++;
	}
	
	return result;
}

static int make_comparison(uint32_t a, uint32_t b, DEB_VERSION_OPERATOR oper) 
{
	int result = 0;
	switch(oper) {
	case DEB_VERSION_EQUAL:
		result = a == b;
		break;
	case DEB_VERSION_EQUAL_OR_HIGHER:
		result = a >= b;
		break;
	case DEB_VERSION_EQUAL_OR_LOWER:
		result = a <= b;
		break;
	case DEB_VERSION_HIGHER:
		result = a > b;
		break;
	case DEB_VERSION_LOWER:
		result = a < b;
		break;
	}

	return result;
}

DEB_VERSION deb_version_new()
{
	DEB_VERSION version = { 0 };
	return version;
}

DEB_VERSION deb_version_create(uint32_t major, uint32_t minor, uint32_t patch, uint32_t build)
{
	DEB_VERSION version = { major, minor, patch, build };
	return version;
}

DEB_VERSION deb_version_parse(const char *str)
{
	if(!str)
		return deb_version_new();

	int len = strlen(str);
	int segment_len = 0;
	int count = 0;

	DEB_VERSION version = deb_version_new();
	uint32_t *segment = &version.major;

	for(int i = 0; i < len; i++) {
		switch(str[i]) {
		case '.': {
			uint32_t num = get_number(&str[i - segment_len], segment_len);

			*segment = num;
			segment++; /* move to next segment */
			
			if(count >= (DEB_VERSION_SEGMENT_COUNT - 1)) {
				return version;
			}

			count++;
			segment_len = 0;
		} break;
		default:
			segment_len++;
		}
	}

	if(segment_len > 0) {
		uint32_t num = get_number(&str[len  - segment_len], segment_len);
		*segment = num;
	}

	return version;
}

int deb_version_compare(DEB_VERSION version_a, DEB_VERSION version_b, DEB_VERSION_OPERATOR oper)
{
	uint32_t *segment_a = &version_a.major;
	uint32_t *segment_b = &version_b.major;

	for(int i = 0; i < DEB_VERSION_SEGMENT_COUNT; i++) {
		if(!make_comparison(*segment_a, *segment_b, oper) && *segment_a != 0 && segment_b != 0) {
			return 0;
		}

		segment_a++;
		segment_b++;
	}

	return 1;
}

int deb_version_is_equal(DEB_VERSION version_a, DEB_VERSION version_b)
{
	return deb_version_compare(version_a, version_b, DEB_VERSION_EQUAL);
}

int deb_version_is_equal_or_higher(DEB_VERSION version_a, DEB_VERSION version_b)
{
	return deb_version_compare(version_a, version_b, DEB_VERSION_EQUAL_OR_HIGHER);
}

int deb_version_is_equal_or_lower(DEB_VERSION version_a, DEB_VERSION version_b)
{
	return deb_version_compare(version_a, version_b, DEB_VERSION_EQUAL_OR_LOWER);
}

int deb_version_is_higher(DEB_VERSION version_a, DEB_VERSION version_b)
{
	return deb_version_compare(version_a, version_b, DEB_VERSION_HIGHER);
}

int deb_version_is_lower(DEB_VERSION version_a, DEB_VERSION version_b)
{
	return deb_version_compare(version_a, version_b, DEB_VERSION_LOWER);
}