#include <glib.h>
#include <stdio.h>
#include <string.h>

#include <debian/libdeb/mem.h>
#include <debian/libdeb/util.h>
#include <debian/libdeb/properties.h>

/* Read: https://www.debian.org/doc/debian-policy/ch-controlfields.html
For more information on the format and rules related to control files. */

DEB_PROPERTIES *
deb_properties_parse(char *str)
{
    DEB_PROPERTIES *properties = g_hash_table_new(g_int_hash, g_int_equal);
    int str_len = strlen(str);

    int propname_len = 0;
    int propval_len = 0;

    int lookin_for_propname = 1;
	int skip_line = 0;

    char *prop_name = NULL;
    char *prop_val = NULL;

	for (int index = 0; index < str_len; index++) {
		char token = str[index];
		char next_token = '\0';
		char prev_token = '\0';

		int is_end = (str_len - 1) == index;
		int is_start = index == 0;

		if (!is_end) {
			next_token = str[index + 1];
		}

		if (!is_start) {
			prev_token = str[index - 1];
		}

        /* stop ignoreing if we've reached the end of a line */
		if (skip_line && token != '\n') {
            skip_line = 0;
			continue;
		}

        if(token == '#') {
            /* start ignoring the rest of the line */
			skip_line = 1;
        } else if(token == ' ' && lookin_for_propname) {
            /* error, property names may not contain spaces */

        } else if(token == ':' && lookin_for_propname) {
            /* read back and copy the name, null terminating it
            to make it a valid string */
            prop_name = DEB_ALLOC(char *, propname_len + 1);
            memcpy(prop_name, str + (index - propname_len), propname_len);
            prop_name[propname_len] = '\0';

            /* make lower case, we are case insensitive */
            strtolower(prop_name);
            
            /* if the property name is followed by a space; ignore it */
            if(next_token == ' ') {
                index++;
            }
          
            /* we'll start looking for the value now */
            lookin_for_propname = 0;

            /* skip increment of counters */
            continue;

        /* if new line starts with a space or tab, we are dealing with a multi-line
        value, continue reading... If the new line does not start with a space or tab
        or we've hit the end of the file, then we've reached the end of this
        property's value */
        } else if(((token == '\n' && next_token != ' ' && next_token != '\t') || is_end) 
            && !lookin_for_propname) {

            /* read back and copy the value, null terminating it to make
            it a valid string */
            prop_val = DEB_ALLOC(char *, propval_len + 1);
            memcpy(prop_val, str + (index - propval_len), propval_len);
            prop_val[propval_len] = '\0';

            /* grab the name and value together and insert into the hash value */
            g_hash_table_insert(properties, prop_name, prop_val);

            /* we'll re-use these variables, do NOT free, the hash table
            now controls those blocks of memory */
            prop_name = NULL;
            prop_val = NULL;

            /* reset states so we can start looking for the next
            property */
            lookin_for_propname = 1;
            propname_len = 0;
            propval_len = 0;

            /* skip increment of counters */
            continue;
        }

        /* increment counters (depending on what we're looking
        for, this allows us to read back if we come across a certain
        token */
        if(lookin_for_propname) {
            propname_len++;
        } else {
            propval_len++;
        }
    }

    return properties;
}

static void
print_property(char *name, char *value, gpointer user_data)
{
    printf("%s: %s\n", name, value);
}

void
deb_properties_print(DEB_PROPERTIES *properties)
{
    if(!properties) {
        return;
    }

    g_hash_table_foreach(properties, print_property, NULL);
}

char *
deb_properties_get(DEB_PROPERTIES *properties, const char *name)
{
    char *value = (char *) g_hash_table_lookup(properties, name);
    return value;
}
