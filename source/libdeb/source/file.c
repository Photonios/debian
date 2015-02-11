#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <archive.h>
#include <archive_entry.h>

#include <debian/libdeb/file.h>
#include <debian/libdeb/mem.h>
#include <debian/libdeb/version.h>

#define DEB_AR_VERSION_FILE_NAME	"debian-binary" /* should be first entry in debian archive (*.deb) */
#define DEB_AR_META_FILE_NAME		"control.tar"   /* contains control file and scripts */
#define DEB_AR_DATA_FILE_NAME		"data.tar"	  /* contains files and data to extract */

static int strstartswith(const char *str, const char *substr)
{
	int substr_len = strlen(substr);
	int result = strncmp(str, substr, substr_len);
	return result == 0 ? 1 : 0;
}

static DEB_RESULT process_version_file(struct archive *debarchive, struct archive_entry *entry)
{
	if(!debarchive || !entry)
		return DEB_RESULT_NULL_PARAM;

	int64_t size = archive_entry_size(entry);

	char *buf = DEB_ALLOC(char *, size + 1);
	if(!buf)
		return DEB_RESULT_NO_MEM;

	if(archive_read_data(debarchive, buf, size) != size) {
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

	free(buf);
	return DEB_RESULT_OK;
}

static DEB_RESULT process_meta_file(struct archive *debarchive, struct archive_entry *entry, DEB_FILE *file)
{
	if(!debarchive || !entry || !file)
		return DEB_RESULT_NULL_PARAM;

	DEB_RESULT result = DEB_RESULT_OK;

	int64_t size = archive_entry_size(entry);
	char *buf = DEB_ALLOC(char *, size);

	struct archive *controlarchive = archive_read_new();
	if(!controlarchive) {
		result = DEB_RESULT_ARCHIVE_FAIL;
		goto cleanup;
	}

	if(archive_read_data(debarchive, buf, size) != size) {
		result = DEB_RESULT_READ_FAIL;
		goto cleanup;
	}

	if(archive_read_open_memory(controlarchive, buf, size) != ARCHIVE_OK) {
		result = DEB_RESULT_META_ARCHIVE_CORRUPT;
		goto cleanup;
	}

	archive_compression(controlarchive);

cleanup:

	return result;
}

DEB_FILE * deb_file_new()
{
	DEB_FILE *new_file = DEB_ALLOC(DEB_FILE *, sizeof(DEB_FILE));
	if(!new_file)
		return NULL; // TODO: Should be DEB_RESULT_NOMEM ?

	deb_file_set_data_compression(new_file, DEB_COMPRESSION_NONE);
	deb_file_set_meta_compression(new_file, DEB_COMPRESSION_NONE);

	return new_file;
}

DEB_RESULT deb_file_open(const char *filename, DEB_FILE **file)
{
	struct archive *debarchive = NULL;
	struct archive_entry *entry = NULL;
	int version_file_found = 0;
	int meta_archive_found = 0;
	int data_archive_found = 0;

	debarchive = archive_read_new();
	if(!debarchive) {	
		goto cleanup;
	}

	*file = deb_file_new();

	/* make sure the build of libarchive supports all formats and compressions */
	if(archive_read_support_format_all(debarchive) != ARCHIVE_OK) {
		goto cleanup;
	}

	if(archive_read_support_compression_all(debarchive) != ARCHIVE_OK) {
		goto cleanup;
	}	

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
		archive_read_finish(debarchive);
	}

	return DEB_RESULT_OK;
}

DEB_RESULT deb_file_set_data_compression(DEB_FILE *file, DEB_COMPRESSION compression)
{
	if(!file)
		return DEB_RESULT_NULL_PARAM;

	file->data_compression = compression;
	return DEB_RESULT_OK;
}

DEB_COMPRESSION deb_file_get_data_compression(DEB_FILE *file)
{
	if(!file)
		return DEB_COMPRESSION_INVALID;

	return file->data_compression;
}

DEB_RESULT deb_file_set_meta_compression(DEB_FILE *file, DEB_COMPRESSION compression)
{
	if(!file)
		return DEB_RESULT_NULL_PARAM;

	file->meta_compression = compression;
	return DEB_RESULT_OK;
}

DEB_COMPRESSION deb_file_get_meta_compression(DEB_FILE *file)
{
	if(!file)
		return DEB_COMPRESSION_INVALID;

	return file->meta_compression;
}