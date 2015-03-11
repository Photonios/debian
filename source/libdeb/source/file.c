#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <archive.h>
#include <archive_entry.h>

#include <debian/libdeb/file.h>
#include <debian/libdeb/mem.h>
#include <debian/libdeb/version.h>
#include <debian/libdeb/properties.h>

/* helps translating ARCHIVE_FILTER_* macros to our internal enum
for various types of compression */
static const __compression_lookup__[DEB_COMPRESSION_COUNT] = {
	DEB_COMPRESSION_NONE,		/* ARCHIVE_FILTER_NONE	0 */
	DEB_COMPRESSION_GZIP,		/* ARCHIVE_FILTER_GZIP	1 */
	DEB_COMPRESSION_BZIP2,		/* ARCHIVE_FILTER_BZIP2 2 */
	DEB_COMPRESSION_INVALID,	/* ARCHIVE_FILTER_COMPRESS 3 */
	DEB_COMPRESSION_INVALID,	/* ARCHIVE_FILTER_PROGRAM 4 */
	DEB_COMPRESSION_LZMA,		/* ARCHIVE_FILTER_LZMA 5 */
	DEB_COMPRESSION_XZ,			/* ARCHIVE_FILTER_XZ 6 */
	DEB_COMPRESSION_UU,			/* ARCHIVE_FILTER_UU 7 */
	DEB_COMPRESSION_RPM,		/* ARCHIVE_FILTER_RPM 8 */
	DEB_COMPRESSION_LZIP,		/* ARCHIVE_FILTER_LZIP 9 */
	DEB_COMPRESSION_LRZIP,		/* ARCHIVE_FILTER_LRZIP 10 */
	DEB_COMPRESSION_INVALID,	/* ARCHIVE_FILTER_LZOP 11 */
	DEB_COMPRESSION_INVALID,	/* ARCHIVE_FILTER_GRZIP 12 */
	DEB_COMPRESSION_INVALID		/* ARCHIVE_FILTER_LZ4 13 */
};

#define DEB_AR_VERSION_FILE_NAME	"debian-binary" /* should be first entry in debian archive (*.deb) */
#define DEB_AR_META_FILE_NAME		"control.tar"   /* contains control file and scripts */
#define DEB_AR_DATA_FILE_NAME		"data.tar"	    /* contains files and data to extract */

#define DEB_CONTROL_FILE_NAME       "control"

static int
strstartswith(const char *str, const char *substr)
{
	int substr_len = strlen(substr);
	int result = strncmp(str, substr, substr_len);
	return result == 0 ? 1 : 0;
}

static struct archive *
prepare_read_new_archive()
{
	struct archive *new_archive = archive_read_new();
	if(!new_archive) {
		return NULL;
	}
	
	if(archive_read_support_format_all(new_archive) != ARCHIVE_OK) {
		return NULL;
	}

	if(archive_read_support_compression_all(new_archive) != ARCHIVE_OK) {
		return NULL;
	}	

	return new_archive;
}

static DEB_COMPRESSION
lookup_compression(int compression)
{
	if(compression > (DEB_COMPRESSION_COUNT - 1) || compression < 0)
		return DEB_COMPRESSION_INVALID;

	return __compression_lookup__[compression];
}

static DEB_ARCHFORMAT
lookup_archformat(int archformat)
{
    /* ugly, but works, probably need to create a proper lookup
    table for this in the future*/
    return (DEB_ARCHFORMAT) archformat;
}

static DEB_RESULT
process_version_file(struct archive *debarchive, struct archive_entry *entry)
{
	if(!debarchive || !entry)
		return DEB_RESULT_NULL_PARAM;

	int64_t size = archive_entry_size(entry);

	char *buf = DEB_ALLOC(char *, size + 1);
	if(!buf)
		return DEB_RESULT_NO_MEM;

	if(archive_read_data(debarchive, buf, (size_t) size) != size) {
		free(buf);
		return DEB_RESULT_READ_FAIL;
	}

	/* make sure it's null-terminated for deb_version_parse */
	buf[size] = '\0';

	DEB_VERSION found_version = deb_version_parse(buf);
	DEB_VERSION required_version = deb_version_create(
		DEB_PACKAGE_FORMAT_VERSION_MAJOR,
		DEB_PACKAGE_FORMAT_VERSION_MINOR,
		DEB_PACKAGE_FORMAT_VERSION_PATCH,
		DEB_PACKAGE_FORMAT_VERSION_BUILD
	);

	if(!deb_version_is_equal(found_version, required_version)) {
		free(buf);
		return DEB_RESULT_INVALID_VERSION;
	}

    if(buf) {
	    free(buf);
    }

	return DEB_RESULT_OK;
}

static DEB_RESULT
parse_control_file(struct archive *controlarchive, struct archive_entry *entry, DEB_FILE *file)
{
    DEB_RESULT result = DEB_RESULT_OK;

    int64_t size = archive_entry_size(entry);
	char *buf = DEB_ALLOC(char *, size + 1); /* +1 for null terminator */

    if(archive_read_data(controlarchive, buf, (size_t) size) != size) {
		result = DEB_RESULT_CONTROL_FILE_READ_FAIL;
		goto cleanup;
	}

    buf[size] = '\0';

    DEB_PROPERTIES *properties = deb_properties_parse(buf);
    if(!properties) {
        result = DEB_RESULT_CONTROL_FILE_PARSE_FAIL;
        goto cleanup;
    }

    file->properties = properties;

cleanup:
    if(buf) {
        free(buf);
    }

    return result;
}

static DEB_RESULT
process_meta_file_contents(struct archive *controlarchive, DEB_FILE *file)
{
    struct archive_entry *entry = NULL;
    int control_file_found = 0;

    while(archive_read_next_header(controlarchive, &entry) == ARCHIVE_OK) {
        /* +2 because all file names start with './' */
		const char *filename = archive_entry_pathname(entry) + 2;

        if(strstartswith(filename, DEB_CONTROL_FILE_NAME)) {
            parse_control_file(controlarchive, entry, file);
            control_file_found = 1;            
        } else {

        }
    }

    return DEB_RESULT_OK;
}

static DEB_RESULT
process_meta_file(struct archive *debarchive, struct archive_entry *entry, DEB_FILE *file)
{
	if(!debarchive || !entry || !file)
		return DEB_RESULT_NULL_PARAM;
    
	DEB_RESULT result = DEB_RESULT_OK;

	int64_t size = archive_entry_size(entry);
	char *buf = DEB_ALLOC(char *, size);

	struct archive *controlarchive = prepare_read_new_archive();
	if(!controlarchive) {
		result = DEB_RESULT_ARCHIVE_FAIL;
		goto cleanup;
	}

	/* the meta data (control.tar.gz) is usually so small that it would
	be a waste of effort to extract it to disk.. do everything in memory :) */

	if(archive_read_data(debarchive, buf, (size_t) size) != size) {
		result = DEB_RESULT_READ_FAIL;
		goto cleanup;
	}
	
	if(archive_read_open_memory(controlarchive, buf, (size_t) size) != ARCHIVE_OK) {		
		result = DEB_RESULT_META_ARCHIVE_CORRUPT;
		goto cleanup;
	}

	DEB_COMPRESSION compression = lookup_compression(
		archive_compression(controlarchive)
	);

	deb_file_set_meta_compression(file, compression);

    DEB_ARCHFORMAT archformat = lookup_archformat(
        archive_format(controlarchive)
    );

    /* because we're reading from memory, libarchive
    will almost always set the archive format to 0,
    default to TAR */
    if(archformat == DEB_ARCHFORMAT_INVALID) {
        archformat = DEB_ARCHFORMAT_TAR;
    }

    deb_file_set_meta_archformat(file, archformat);

    /* extract control file and scripts */
    result = process_meta_file_contents(controlarchive, file);
    if(deb_result_is_bad(result)) {
        goto cleanup;
    }

cleanup:
    if(controlarchive) {
        archive_read_close(controlarchive);
    }

	if(buf) {
		free(buf);        
	}

	return result;
}

DEB_FILE *
deb_file_new()
{
	DEB_FILE *new_file = DEB_ALLOC(DEB_FILE *, sizeof(DEB_FILE));
	if(!new_file)
		return NULL; // TODO: Should be DEB_RESULT_NOMEM ?

	deb_file_set_data_compression(new_file, DEB_COMPRESSION_NONE);
	deb_file_set_meta_compression(new_file, DEB_COMPRESSION_NONE);

	return new_file;
}

DEB_RESULT
deb_file_open(const char *filename, DEB_FILE **file)
{
	struct archive *debarchive = NULL;
	struct archive_entry *entry = NULL;
	int version_file_found = 0;
	int meta_archive_found = 0;
	int data_archive_found = 0;

	debarchive = prepare_read_new_archive();
	if(!debarchive) {	
		goto cleanup;
	}

	*file = deb_file_new();

	/* open the deb file for reading */
	if(archive_read_open_filename(debarchive, filename, 10240) != ARCHIVE_OK) {
		goto cleanup;
	}

	while(archive_read_next_header(debarchive, &entry) == ARCHIVE_OK) {
		const char *filename = archive_entry_pathname(entry);

		/* version file, verify debian package format version */
		if(!version_file_found && strstartswith(filename, DEB_AR_VERSION_FILE_NAME)) {
			DEB_RESULT res = process_version_file(debarchive, entry);
			if(deb_result_is_bad(res)) {
				return res;
			}
			
			version_file_found = 1;

		/* meta data file (contains scripts and package info) */
		} else if(!meta_archive_found && strstartswith(filename, DEB_AR_META_FILE_NAME)) {
			DEB_RESULT res = process_meta_file(debarchive, entry, *file);
			if(deb_result_is_bad(res)) {
				return res;
			}

			meta_archive_found = 1;

		/* data file, contains all files to extract */
		} else if(!data_archive_found && strstartswith(filename, DEB_AR_DATA_FILE_NAME)) {
			
		}
	}

cleanup:
	if(debarchive) {
		archive_read_close(debarchive);
	}

	return DEB_RESULT_OK;
}

DEB_RESULT
deb_file_set_data_compression(DEB_FILE *file, DEB_COMPRESSION compression)
{
	if(!file)
		return DEB_RESULT_NULL_PARAM;

	file->data_compression = compression;
	return DEB_RESULT_OK;
}

DEB_COMPRESSION
deb_file_get_data_compression(DEB_FILE *file)
{
	if(!file)
		return DEB_COMPRESSION_INVALID;

	return file->data_compression;
}

DEB_RESULT
deb_file_set_meta_compression(DEB_FILE *file, DEB_COMPRESSION compression)
{
	if(!file)
		return DEB_RESULT_NULL_PARAM;

	file->meta_compression = compression;
	return DEB_RESULT_OK;
}

DEB_COMPRESSION
deb_file_get_meta_compression(DEB_FILE *file)
{
	if(!file)
		return DEB_COMPRESSION_INVALID;

	return file->meta_compression;
}

DEB_RESULT
deb_file_set_data_archformat(DEB_FILE *file, DEB_ARCHFORMAT format)
{
    if(!file)
        return DEB_RESULT_NULL_PARAM;

    file->data_archformat = format;
    return DEB_RESULT_OK;
}

DEB_ARCHFORMAT
deb_file_get_data_archformat(DEB_FILE *file)
{
    if(!file)
        return DEB_ARCHFORMAT_INVALID;

    return file->data_archformat;
}

DEB_RESULT
deb_file_set_meta_archformat(DEB_FILE *file, DEB_ARCHFORMAT format)
{
    if(!file)
        return DEB_RESULT_NULL_PARAM;

    file->meta_archformat = format;
    return DEB_RESULT_OK;
}

DEB_ARCHFORMAT
deb_file_get_meta_archformat(DEB_FILE *file)
{
    if(!file)
        return DEB_ARCHFORMAT_INVALID;

    return file->meta_archformat;
}