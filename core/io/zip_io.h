#pragma once

#include "core/io/file_access.h"

// Not directly used in this header, but assumed available in downstream users
// like platform/*/export/export.cpp. Could be fixed, but probably better to have
// thirdparty includes in as little headers as possible.
#include "thirdparty/minizip/unzip.h"
#include "thirdparty/minizip/zip.h"

// Get the current file info and safely convert the full filepath to a String.
int godot_unzip_get_current_file_info(unzFile p_zip_file, unz_file_info64 &r_file_info, String &r_filepath);
// Try to locate the file in the archive specified by the filepath (works with large paths and Unicode).
int godot_unzip_locate_file(unzFile p_zip_file, const String &p_filepath, bool p_case_sensitive = true);

//

void *zipio_open(voidpf opaque, const char *p_fname, int mode);
uLong zipio_read(voidpf opaque, voidpf stream, void *buf, uLong size);
uLong zipio_write(voidpf opaque, voidpf stream, const void *buf, uLong size);

long zipio_tell(voidpf opaque, voidpf stream);
long zipio_seek(voidpf opaque, voidpf stream, uLong offset, int origin);

int zipio_close(voidpf opaque, voidpf stream);

int zipio_testerror(voidpf opaque, voidpf stream);

voidpf zipio_alloc(voidpf opaque, uInt items, uInt size);
void zipio_free(voidpf opaque, voidpf address);

zlib_filefunc_def zipio_create_io(Ref<FileAccess> *p_data);
