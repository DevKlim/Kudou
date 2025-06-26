#pragma once

#include "core/io/file_access.h"
#include "core/object/ref_counted.h"

#include "thirdparty/minizip/zip.h"

class ZIPPacker : public RefCounted {
	GDCLASS(ZIPPacker, RefCounted);

	Ref<FileAccess> fa;
	zipFile zf = nullptr;
	int compression_level = Z_DEFAULT_COMPRESSION;

protected:
	static void _bind_methods();

public:
	enum ZipAppend {
		APPEND_CREATE = 0,
		APPEND_CREATEAFTER = 1,
		APPEND_ADDINZIP = 2,
	};

	enum CompressionLevel {
		COMPRESSION_DEFAULT = Z_DEFAULT_COMPRESSION,
		COMPRESSION_NONE = Z_NO_COMPRESSION,
		COMPRESSION_FAST = Z_BEST_SPEED,
		COMPRESSION_BEST = Z_BEST_COMPRESSION,
	};

	Error open(const String &p_path, ZipAppend p_append);
	Error close();

	void set_compression_level(int p_compression_level);
	int get_compression_level() const;

	Error start_file(const String &p_path);
	Error write_file(const Vector<uint8_t> &p_data);
	Error close_file();

	ZIPPacker();
	~ZIPPacker();
};

VARIANT_ENUM_CAST(ZIPPacker::ZipAppend)
VARIANT_ENUM_CAST(ZIPPacker::CompressionLevel)
