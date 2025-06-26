#pragma once

#include "core/io/file_access.h"
#include "core/object/ref_counted.h"

#include "thirdparty/minizip/unzip.h"

class ZIPReader : public RefCounted {
	GDCLASS(ZIPReader, RefCounted)

	Ref<FileAccess> fa;
	unzFile uzf = nullptr;

protected:
	static void _bind_methods();

public:
	Error open(const String &p_path);
	Error close();

	PackedStringArray get_files();
	PackedByteArray read_file(const String &p_path, bool p_case_sensitive);
	bool file_exists(const String &p_path, bool p_case_sensitive);
	int get_compression_level(const String &p_path, bool p_case_sensitive);

	ZIPReader();
	~ZIPReader();
};
