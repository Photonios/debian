#include <glib.h>
#include <stdio.h>
#include <string.h>

#include <debian/libdeb/mem.h>
#include <debian/libdeb/properties.h>

GHashTable *
deb_property_list_parse(char *str)
{
    GHashTable *properties = g_hash_table_new(g_int_hash, g_int_equal);
    int str_len = strlen(str);

    int propname_len = 0;
    int propval_len = 0;

    int lookin_for_propname = 1;

    char *prop_name = NULL;
    char *prop_val = NULL;

    for(int index = 0; index < str_len; index++) {
        char token = str[index];

        if(token == ':' && lookin_for_propname) {
            /* read back and copy the name, null terminating it
            to make it a valid string */
            prop_name = DEB_ALLOC(char *, propname_len + 1);
            memcpy(prop_name, str + (index - propname_len), propname_len);
            prop_name[propname_len] = '\0';                    
            
            /* if the property name is followed by a space; ignore it */
            if(str[index + 1] == ' ') {
                index++;
            }
          
            /* we'll start looking for the value now */
            lookin_for_propname = 0;

            /* skip increment of counters */
            continue;

        /* if new line starts with a space, we are dealing with a multi-line
        value, continue reading... If the new line does not start with a space
        or we've hit the end of the file, then we've reached the end of this
        property's value */
        } else if(((token == '\n' && str[index + 1] != ' ') || (str_len - 1) == index) 
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

