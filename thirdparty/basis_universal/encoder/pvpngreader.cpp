// pngreader.cpp - Public Domain - see unlicense at bottom of file.
//
// Notes: 
// This is ancient code from ~1995 ported to C++. It was originally written for a 
// DOS app with very limited memory. It's not as fast as it should be, but it works. 
// The low-level PNG reader class was written assuming the PNG file could not fit 
// entirely into memory, which dictated how it was written/structured.
// It has been modified to use either zlib or miniz.
// It supports all PNG color types/bit depths/interlacing, however 16-bit/component 
// images are converted to 8-bit.
// TRNS chunks are converted to alpha as needed.
// GAMA chunk is read, but not applied.

#include "../transcoder/basisu.h"

#define MINIZ_HEADER_FILE_ONLY
#define MINIZ_NO_ZLIB_COMPATIBLE_NAMES
#include "basisu_miniz.h"

#include "pvpngreader.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <assert.h>

#define PVPNG_IDAT_CRC_CHECKING (1)
#define PVPNG_ADLER32_CHECKING (1)

namespace pv_png
{

const uint32_t MIN_PNG_SIZE = 8 + 13 + 8 + 1 + 4 + 12;

template <typename S> inline S maximum(S a, S b) { return (a > b) ? a : b; }
template <typename S> inline S minimum(S a, S b) { return (a < b) ? a : b; }

template <typename T> inline void clear_obj(T& obj) { memset(&obj, 0, sizeof(obj)); }

#define MAX_SUPPORTED_RES (32768)
#define FALSE (0)
#define TRUE (1)
#define PNG_MAX_ALLOC_BLOCKS (16)

enum
{
	PNG_DECERROR = -3,
	PNG_ALLDONE = -5,
	PNG_READPASTEOF = -11,
	PNG_UNKNOWNTYPE = -16,
	PNG_FILEREADERROR = -17,
	PNG_NOTENOUGHMEM = -108,
	PNG_BAD_CHUNK_CRC32 = -13000,
	PNG_NO_IHDR = -13001,
	PNG_BAD_WIDTH = -13002,
	PNG_BAD_HEIGHT = -13003,
	PNG_UNS_COMPRESSION = -13004,
	PNG_UNS_FILTER = -13005,
	PNG_UNS_ILACE = -13006,
	PNG_UNS_COLOR_TYPE = -13007,
	PNG_BAD_BIT_DEPTH = -13008,
	PNG_BAD_CHUNK_SIZE = -13009,
	PNG_UNS_CRITICAL_CHUNK = -13010,
	PNG_BAD_TRNS_CHUNK = -13011,
	PNG_BAD_PLTE_CHUNK = -13012,
	PNG_UNS_RESOLUTION = -13013,
	PNG_INVALID_DATA_STREAM = -13014,
	PNG_MISSING_PALETTE = -13015,
	PNG_UNS_PREDICTOR = -13016,
	PNG_INCOMPLETE_IMAGE = -13017,
	PNG_TOO_MUCH_DATA = -13018
};

#define PNG_COLOR_TYPE_PAL_MASK        (1)
#define PNG_COLOR_TYPE_COL_MASK        (2)
#define PNG_COLOR_TYPE_ALP_MASK        (4)

#define PNG_INFLATE_SRC_BUF_SIZE       (4096)

struct ihdr_struct
{
	uint32_t m_width;
	uint32_t m_height;
	uint8_t m_bit_depth;
	uint8_t m_color_type;
	uint8_t m_comp_type;
	uint8_t m_filter_type;
	uint8_t m_ilace_type;
};

class png_file
{
public:
	png_file() { }
	virtual ~png_file() { }

	virtual bool resize(uint64_t new_size) = 0;
	virtual uint64_t get_size() = 0;
	virtual uint64_t tell() = 0;
	virtual bool seek(uint64_t ofs) = 0;
	virtual size_t write(const void* pBuf, size_t len) = 0;
	virtual size_t read(void* pBuf, size_t len) = 0;
};

class png_memory_file : public png_file
{
public:
	std::vector<uint8_t> m_buf;
	uint64_t m_ofs;
	
	png_memory_file() : 
		png_file(),
		m_ofs(0)
	{ 
	}
	
	virtual ~png_memory_file()
	{ 
	}

	std::vector<uint8_t>& get_buf() { return m_buf; }
	const std::vector<uint8_t>& get_buf() const { return m_buf; }
	 
	void init()
	{
		m_ofs = 0;
		m_buf.resize(0);
	}

	virtual bool resize(uint64_t new_size)
	{
		if ((sizeof(size_t) == sizeof(uint32_t)) && (new_size >= 0x7FFFFFFF))
			return false;

		m_buf.resize((size_t)new_size);
		m_ofs = m_buf.size();

		return true;
	}

	virtual uint64_t get_size()
	{
		return m_buf.size();
	}

	virtual uint64_t tell()
	{
		return m_ofs;
	}

	virtual bool seek(uint64_t ofs)
	{
		m_ofs = ofs;
		return true;
	}

	virtual size_t write(const void* pBuf, size_t len)
	{
		uint64_t new_size = m_ofs + len;
		if (new_size > m_buf.size())
		{
			if ((sizeof(size_t) == sizeof(uint32_t)) && (new_size > 0x7FFFFFFFUL))
				return 0;
			m_buf.resize((size_t)new_size);
		}

		memcpy(&m_buf[(size_t)m_ofs], pBuf, len);
		m_ofs += len;

		return len;
	}

	virtual size_t read(void* pBuf, size_t len)
	{
		if (m_ofs >= m_buf.size())
			return 0;

		uint64_t max_bytes = minimum<uint64_t>(len, m_buf.size() - m_ofs);
		memcpy(pBuf, &m_buf[(size_t)m_ofs], (size_t)max_bytes);

		m_ofs += max_bytes;

		return (size_t)max_bytes;
	}
};

class png_readonly_memory_file : public png_file
{
public:
	const uint8_t* m_pBuf;
	size_t m_buf_size;
	uint64_t m_ofs;

	png_readonly_memory_file() :
		png_file(),
		m_pBuf(nullptr),
		m_buf_size(0),
		m_ofs(0)
	{
	}

	virtual ~png_readonly_memory_file()
	{
	}

	void init(const void *pBuf, size_t buf_size)
	{
		m_pBuf = static_cast<const uint8_t*>(pBuf);
		m_buf_size = buf_size;
		m_ofs = 0;
	}

	virtual bool resize(uint64_t new_size)
	{
		(void)new_size;
		assert(0);
		return false;
	}

	virtual uint64_t get_size()
	{
		return m_buf_size;
	}

	virtual uint64_t tell()
	{
		return m_ofs;
	}

	virtual bool seek(uint64_t ofs)
	{
		m_ofs = ofs;
		return true;
	}

	virtual size_t write(const void* pBuf, size_t len)
	{
		(void)pBuf;
		(void)len;
		assert(0);
		return 0;
	}

	virtual size_t read(void* pBuf, size_t len)
	{
		if (m_ofs >= m_buf_size)
			return 0;

		uint64_t max_bytes = minimum<uint64_t>(len, m_buf_size - m_ofs);
		memcpy(pBuf, &m_pBuf[(size_t)m_ofs], (size_t)max_bytes);

		m_ofs += max_bytes;

		return (size_t)max_bytes;
	}
};

#ifdef _MSC_VER
#define ftell64 _ftelli64
#define fseek64 _fseeki64
#else
#define ftell64 ftello
#define fseek64 fseeko
#endif

class png_cfile : public png_file
{
public:
	FILE* m_pFile;
	
	png_cfile() : 
		png_file(),
		m_pFile(nullptr)
	{
	}

	virtual ~png_cfile()
	{
		close();
	}

	bool init(const char *pFilename, const char *pMode)
	{
		close();
		
		m_pFile = nullptr;
		
#ifdef _MSC_VER
		fopen_s(&m_pFile, pFilename, pMode);
#else
		m_pFile = fopen(pFilename, pMode);
#endif

		return m_pFile != nullptr;
	}

	bool close()
	{
		bool status = true;
		if (m_pFile)
		{
			if (fclose(m_pFile) == EOF)
				status = false;
			m_pFile = nullptr;
		}
		return status;
	}

	virtual bool resize(uint64_t new_size)
	{
		if (new_size)
		{
			if (!seek(new_size - 1))
				return false;

			int v = 0;
			if (write(&v, 1) != 1)
				return false;
		}
		else
		{
			if (!seek(0))
				return false;
		}

		return true;
	}

	virtual uint64_t get_size()
	{
		int64_t cur_ofs = ftell64(m_pFile);
		if (cur_ofs < 0)
			return 0;
		
		if (fseek64(m_pFile, 0, SEEK_END) != 0)
			return 0;
		
		const int64_t cur_size = ftell64(m_pFile);
		if (cur_size < 0)
			return 0;

		if (fseek64(m_pFile, cur_ofs, SEEK_SET) != 0)
			return 0;
		
		return cur_size;
	}

	virtual uint64_t tell()
	{
		int64_t cur_ofs = ftell64(m_pFile);
		if (cur_ofs < 0)
			return 0;

		return cur_ofs;
	}

	virtual bool seek(uint64_t ofs)
	{
		return fseek64(m_pFile, ofs, SEEK_SET) == 0;
	}

	virtual size_t write(const void* pBuf, size_t len)
	{
		return (size_t)fwrite(pBuf, 1, len, m_pFile);
	}

	virtual size_t read(void* pBuf, size_t len)
	{
		return (size_t)fread(pBuf, 1, len, m_pFile);
	}
};

// This low-level helper class handles the actual decoding of PNG files.
class png_decoder
{
public:
	png_decoder();
	~png_decoder();

	// Scans the PNG file, but doesn't decode the IDAT data. 
	// Returns 0 on success, or an error code.
	// If the returned status is non-zero, or m_img_supported_flag==FALSE the image either the image is corrupted/not PNG or is unsupported in some way.
	int png_scan(png_file *pFile);

	// Decodes a single scanline of PNG image data.
	// Returns a pointer to the scanline's pixel data and its size in bytes. 
	// This data is only minimally processed from the internal PNG pixel data.
	// The caller must use the ihdr, trns_flag and values, and the palette to actually decode the pixel data.
	//
	// Possible returned pixel formats is somewhat complex due to the history of this code:
	// 8-bit RGBA, always 4 bytes/pixel - 24bpp PNG's are converted to 32bpp and TRNS processing is done automatically (8/16bpp RGB or RGBA PNG files)
	// 1/2/4/8-bit grayscale, 1 byte per pixel - must convert to [0,255] using the palette or some other means, must optionally use the TRNS chunk for alpha (1/2/4/8 Grayscale PNG files - not 16bpp though!)
	// 1/2/4/8-bit palettized, 1 byte per pixel - must convert to RGB using the 24bpp palette and optionally the TRNS chunk for alpha (1/2/4/8bpp palettized PNG files)
	// 8-bit grayscale with alpha, 2 bytes per pixel - TRNS processing will be done for you on 16bpp images (there's a special case here for 16bpp Grey files) (8/16bpp Gray-Alpha *or 16bpp Grayscale* PNG files)
	//
	// Returns 0 on success, a non-zero error code, or PNG_ALLDONE.
	int png_decode(void** ppImg_ptr, uint32_t* pImg_len);
	
	// Starts decoding. Returns 0 on success, otherwise an error code.
	int png_decode_start();
	
	// Deinitializes the decoder, freeing all allocations.
	void png_decode_end();

	png_file* m_pFile;
		
	// Image's 24bpp palette - 3 bytes per entry
	uint8_t m_plte_flag;
	uint8_t m_img_pal[768];
		
	int m_img_supported_flag;
		
	ihdr_struct m_ihdr;

	uint8_t m_chunk_flag;
	uint32_t m_chunk_size;
	uint32_t m_chunk_left;
	uint32_t m_chunk_crc32;
	uint8_t m_chunk_name[4];

	uint8_t m_end_of_idat_chunks;

	void* m_pMalloc_blocks[PNG_MAX_ALLOC_BLOCKS];

	uint32_t m_dec_bytes_per_pixel; // bytes per pixel decoded from the PNG file (minimum 1 for 1/2/4 bpp), factors in the PNG 8/16 bit/component bit depth, may be up to 8 bytes (2*4)
	uint32_t m_dst_bytes_per_pixel; // bytes per pixel returned to the caller (1-4), always has alpha if the PNG has alpha, 16-bit components always converted to 8-bits/component

	uint32_t m_dec_bytes_per_line;	// bytes per line decoded from the PNG file (before 1/2/4 expansion), +1 for the filter byte
	uint32_t m_src_bytes_per_line;	// decoded PNG bytes per line, before 1/2/4 bpp expansion, not counting the filter byte, updated during adam7 deinterlacing
	uint32_t m_dst_bytes_per_line;	// bytes per line returned to the caller (1-4 times width)

	int (*m_pProcess_func)(uint8_t* src, uint8_t* dst, int pixels, png_decoder* pwi);

	uint8_t* m_pPre_line_buf;
	uint8_t* m_pCur_line_buf;
	uint8_t* m_pPro_line_buf;

	uint8_t m_bkgd_flag;
	uint32_t  m_bkgd_value[3];

	uint8_t m_gama_flag;
	uint32_t m_gama_value;
			
	uint8_t m_trns_flag;
	uint32_t m_trns_value[256];

	buminiz::mz_stream m_inflator;

	uint8_t inflate_src_buf[PNG_INFLATE_SRC_BUF_SIZE];

	uint32_t m_inflate_src_buf_ofs;
	uint32_t m_inflate_src_buf_size;
	uint32_t m_inflate_dst_buf_ofs;

	int m_inflate_eof_flag;
		
	uint8_t m_gamma_table[256];

	int m_pass_x_size;
	int m_pass_y_left;

	int m_adam7_pass_num;
	int m_adam7_pass_y;
	int m_adam7_pass_size_x[7];
	int m_adam7_pass_size_y[7];

	std::vector<uint8_t> m_adam7_image_buf;
		
	int m_adam7_decoded_flag;
	
	bool m_scanned_flag;
		
	int m_terminate_status;
		
#define TEMP_BUF_SIZE (384)
	uint8_t m_temp_buf[TEMP_BUF_SIZE * 4];
			
	void clear();
	void uninitialize();
	int terminate(int status);
	void* png_malloc(uint32_t i);
	void* png_calloc(uint32_t i);
	int block_read(void* buf, uint32_t len);
	int64_t block_read_dword();
	int fetch_next_chunk_data(uint8_t* buf, int bytes);
	int fetch_next_chunk_byte();
	int fetch_next_chunk_word();
	int64_t fetch_next_chunk_dword();
	int fetch_next_chunk_init();
	int unchunk_data(uint8_t* buf, uint32_t bytes, uint32_t* ptr_bytes_read);
	inline void adam7_write_pixel_8(int x, int y, int c);
	inline void adam7_write_pixel_16(int x, int y, int r, int g);
	inline void adam7_write_pixel_24(int x, int y, int r, int g, int b);
	inline void adam7_write_pixel_32(int x, int y, int r, int g, int b, int a);
	void unpredict_sub(uint8_t* lst, uint8_t* cur, uint32_t bytes, int bpp);
	void unpredict_up(uint8_t* lst, uint8_t* cur, uint32_t bytes, int bpp);
	void unpredict_average(uint8_t* lst, uint8_t* cur, uint32_t bytes, int bpp);
	inline uint8_t paeth_predictor(int a, int b, int c);
	void unpredict_paeth(uint8_t* lst, uint8_t* cur, uint32_t bytes, int bpp);
	int adam7_pass_size(int size, int start, int step);
	int decompress_line(uint32_t* bytes_decoded);
	int find_iend_chunk();
	void calc_gamma_table();
	void create_grey_palette();
	int read_signature();
	int read_ihdr_chunk();
	int read_bkgd_chunk();
	int read_gama_chunk();
	int read_trns_chunk();
	int read_plte_chunk();
	int find_idat_chunk();
};

void png_decoder::uninitialize()
{
	m_pFile = nullptr;
				
	for (int i = 0; i < PNG_MAX_ALLOC_BLOCKS; i++)
	{
		free(m_pMalloc_blocks[i]);
		m_pMalloc_blocks[i] = nullptr;
	}

	mz_inflateEnd(&m_inflator);
}

int png_decoder::terminate(int status)
{
	if (m_terminate_status == 0)
		m_terminate_status = status;

	uninitialize();
	return status;
}

void* png_decoder::png_malloc(uint32_t len)
{
	if (!len)
		len++;

	void* p = malloc(len);

	if (!p)
		return nullptr;

	int j;
	for (j = 0; j < PNG_MAX_ALLOC_BLOCKS; j++)
		if (!m_pMalloc_blocks[j])
			break;

	if (j == PNG_MAX_ALLOC_BLOCKS)
		return nullptr;

	m_pMalloc_blocks[j] = p;

	return p;
}

void* png_decoder::png_calloc(uint32_t len)
{
	void* p = png_malloc(len);
	if (!p)
		return nullptr;

	if (p)
		memset(p, 0, len);

	return p;
}

int png_decoder::block_read(void* buf, uint32_t len)
{
	size_t bytes_read = m_pFile->read(buf, len);
	if (bytes_read != len)
		return terminate(PNG_READPASTEOF);
	return 0;
}

int64_t png_decoder::block_read_dword()
{
	uint8_t buf[4];

	int status = block_read(buf, 4);
	if (status != 0)
		return status;

	uint32_t v = buf[3] + ((uint32_t)buf[2] << 8) + ((uint32_t)buf[1] << 16) + ((uint32_t)buf[0] << 24);
	return (int64_t)v;
}

int png_decoder::fetch_next_chunk_data(uint8_t* buf, int bytes)
{
	if (!m_chunk_flag)
		return 0;

	bytes = minimum<int>(bytes, m_chunk_left);

	int status = block_read(buf, bytes);
	if (status != 0)
		return status;
				
#if PVPNG_IDAT_CRC_CHECKING
	bool check_crc32 = true;
#else
	const bool is_idat = (m_chunk_name[0] == 'I') && (m_chunk_name[1] == 'D') && (m_chunk_name[2] == 'A') && (m_chunk_name[3] == 'T');
	bool check_crc32 = !is_idat;
#endif

	if (check_crc32)
		m_chunk_crc32 = buminiz::mz_crc32(m_chunk_crc32, buf, bytes);

	if ((m_chunk_left -= bytes) == 0)
	{
		int64_t res = block_read_dword();
		if (res < 0)
			return (int)res;

		if (check_crc32)
		{
			if (m_chunk_crc32 != (uint32_t)res)
				return terminate(PNG_BAD_CHUNK_CRC32);
		}

		m_chunk_flag = FALSE;
	}

	return bytes;
}

int png_decoder::fetch_next_chunk_byte()
{
	uint8_t buf[1];

	int status = fetch_next_chunk_data(buf, 1);
	if (status < 0)
		return status;

	if (status != 1)
		return terminate(PNG_BAD_CHUNK_SIZE);

	return buf[0];
}

int png_decoder::fetch_next_chunk_word()
{
	uint8_t buf[2];

	int status = fetch_next_chunk_data(buf, 2);
	if (status < 0)
		return status;

	if (status != 2)
		return terminate(PNG_BAD_CHUNK_SIZE);

	return buf[1] + ((uint32_t)buf[0] << 8);
}

int64_t png_decoder::fetch_next_chunk_dword()
{
	uint8_t buf[4];

	int status = fetch_next_chunk_data(buf, 4);
	if (status < 0)
		return status;

	if (status != 4)
		terminate(PNG_BAD_CHUNK_SIZE);

	uint32_t v = buf[3] + ((uint32_t)buf[2] << 8) + ((uint32_t)buf[1] << 16) + ((uint32_t)buf[0] << 24);
	return (int64_t)v;
}

int png_decoder::fetch_next_chunk_init()
{
	while (m_chunk_flag)
	{
		int status = fetch_next_chunk_data(m_temp_buf, TEMP_BUF_SIZE * 4);
		if (status != 0)
			return status;
	}
	
	int64_t n = block_read_dword();
	if (n < 0)
		return (int)n;

	m_chunk_size = (uint32_t)n;

	m_chunk_flag = TRUE;
	m_chunk_left = m_chunk_size + 4;
	m_chunk_crc32 = 0;

	int status = fetch_next_chunk_data(m_chunk_name, 4);
	if (status < 0)
		return status;

	return 0;
}

int png_decoder::unchunk_data(uint8_t* buf, uint32_t bytes, uint32_t* ptr_bytes_read)
{
	uint32_t bytes_read = 0;

	if ((!bytes) || (m_end_of_idat_chunks))
	{
		*ptr_bytes_read = 0;
		return TRUE;
	}

	while (bytes_read != bytes)
	{
		if (!m_chunk_flag)
		{
			int res = fetch_next_chunk_init();
			if (res < 0)
				return res;

			if ((m_chunk_name[0] != 'I') ||
				(m_chunk_name[1] != 'D') ||
				(m_chunk_name[2] != 'A') ||
				(m_chunk_name[3] != 'T'))
			{
				*ptr_bytes_read = bytes_read;
				m_end_of_idat_chunks = TRUE;
				return TRUE;
			}
		}

		int res = fetch_next_chunk_data(buf + bytes_read, bytes - bytes_read);
		if (res < 0)
			return res;

		bytes_read += (uint32_t)res;
	}

	*ptr_bytes_read = bytes_read;

	return FALSE;
}

inline void png_decoder::adam7_write_pixel_8(int x, int y, int c)
{
	m_adam7_image_buf[x + y * m_dst_bytes_per_line] = (uint8_t)c;
}

inline void png_decoder::adam7_write_pixel_16(int x, int y, int r, int g)
{
	uint32_t ofs = x * 2 + y * m_dst_bytes_per_line;
	m_adam7_image_buf[ofs + 0] = (uint8_t)r;
	m_adam7_image_buf[ofs + 1] = (uint8_t)g;
}

inline void png_decoder::adam7_write_pixel_24(int x, int y, int r, int g, int b)
{
	uint32_t ofs = x * 3 + y * m_dst_bytes_per_line;
	m_adam7_image_buf[ofs + 0] = (uint8_t)r;
	m_adam7_image_buf[ofs + 1] = (uint8_t)g;
	m_adam7_image_buf[ofs + 2] = (uint8_t)b;
}

inline void png_decoder::adam7_write_pixel_32(int x, int y, int r, int g, int b, int a)
{
	uint32_t ofs = x * 4 + y * m_dst_bytes_per_line;
	m_adam7_image_buf[ofs + 0] = (uint8_t)r;
	m_adam7_image_buf[ofs + 1] = (uint8_t)g;
	m_adam7_image_buf[ofs + 2] = (uint8_t)b;
	m_adam7_image_buf[ofs + 3] = (uint8_t)a;
}

static void PixelDePack2(void* src, void* dst, int numbytes)
{
	uint8_t* src8 = (uint8_t*)src;
	uint8_t* dst8 = (uint8_t*)dst;

	while (numbytes)
	{
		uint8_t v = *src8++;
		
		for (uint32_t i = 0; i < 8; i++)
			dst8[7 - i] = (v >> i) & 1;

		dst8 += 8;
		numbytes--;
	}
}

static void PixelDePack16(void* src, void* dst, int numbytes)
{
	uint8_t* src8 = (uint8_t*)src;
	uint8_t* dst8 = (uint8_t*)dst;

	while (numbytes)
	{
		uint8_t v = *src8++;

		dst8[0] = (uint8_t)v >> 4;
		dst8[1] = (uint8_t)v & 0xF;
		dst8 += 2;

		numbytes--;
	}
}

static int unpack_grey_1(uint8_t* src, uint8_t* dst, int pixels, png_decoder *pwi)
{
	(void)pwi;
	PixelDePack2(src, dst, pixels >> 3);

	dst += (pixels & 0xFFF8);

	if ((pixels & 7) != 0)
	{
		uint8_t c = src[pixels >> 3];

		pixels &= 7;

		while (pixels--)
		{
			*dst++ = ((c & 128) >> 7);

			c <<= 1;
		}
	}

	return TRUE;
}

static int unpack_grey_2(uint8_t* src, uint8_t* dst, int pixels, png_decoder* pwi)
{
	(void)pwi;
	int i = pixels;
	uint8_t c;

	while (i >= 4)
	{
		c = *src++;

		*dst++ = (c >> 6);
		*dst++ = (c >> 4) & 3;
		*dst++ = (c >> 2) & 3;
		*dst++ = (c) & 3;

		i -= 4;
	}

	if (i)
	{
		c = *src;

		while (i--)
		{
			*dst++ = (c >> 6);

			c <<= 2;
		}
	}

	return TRUE;
}

static int unpack_grey_4(uint8_t* src, uint8_t* dst, int pixels, png_decoder* pwi)
{
	(void)pwi;

	PixelDePack16(src, dst, pixels >> 1);

	if (pixels & 1)
		dst[pixels & 0xFFFE] = (src[pixels >> 1] >> 4);

	return TRUE;
}

static int unpack_grey_8(uint8_t* src, uint8_t* dst, int pixels, png_decoder* pwi)
{
	(void)src;
	(void)dst;
	(void)pixels;
	(void)pwi;
	return FALSE;
}

static int unpack_grey_16(uint8_t* src, uint8_t* dst, int pixels, png_decoder* pwi)
{
	(void)pwi;
	while (pixels--)
	{
		*dst++ = *src++;

		src++;
	}

	return TRUE;
}

static int unpack_grey_16_2(uint8_t* src, uint8_t* dst, int pixels, png_decoder* pwi)
{
	if (pwi->m_trns_flag)
	{
		while (pixels--)
		{
			uint32_t v = (src[0] << 8) + src[1];
			src += 2;

			*dst++ = (uint8_t)(v >> 8);
			*dst++ = (v == pwi->m_trns_value[0]) ? 0 : 255;
		}
	}
	else
	{
		while (pixels--)
		{
			*dst++ = *src++;
			*dst++ = 0xFF;

			src++;
		}
	}

	return TRUE;
}

static int unpack_true_8(uint8_t* src, uint8_t* dst, int pixels, png_decoder* pwi)
{
	if (pwi->m_trns_flag)
	{
		const uint32_t tr = pwi->m_trns_value[0];
		const uint32_t tg = pwi->m_trns_value[1];
		const uint32_t tb = pwi->m_trns_value[2];

		for (int i = 0; i < pixels; i++)
		{
			uint8_t r = src[i * 3 + 0];
			uint8_t g = src[i * 3 + 1];
			uint8_t b = src[i * 3 + 2];
			
			dst[i * 4 + 0] = r;
			dst[i * 4 + 1] = g;
			dst[i * 4 + 2] = b;
			dst[i * 4 + 3] = ((r == tr) && (g == tg) && (b == tb)) ? 0 : 255;
		}
	}
	else
	{
		for (int i = 0; i < pixels; i++)
		{
			dst[i * 4 + 0] = src[i * 3 + 0];
			dst[i * 4 + 1] = src[i * 3 + 1];
			dst[i * 4 + 2] = src[i * 3 + 2];
			dst[i * 4 + 3] = 255;
		}
	}

	return TRUE;
}

static int unpack_true_16(uint8_t* src, uint8_t* dst, int pixels, png_decoder* pwi)
{
	if (pwi->m_trns_flag)
	{
		const uint32_t tr = pwi->m_trns_value[0];
		const uint32_t tg = pwi->m_trns_value[1];
		const uint32_t tb = pwi->m_trns_value[2];

		for (int i = 0; i < pixels; i++)
		{
			uint32_t r = (src[i * 6 + 0] << 8) + src[i * 6 + 1];
			uint32_t g = (src[i * 6 + 2] << 8) + src[i * 6 + 3];
			uint32_t b = (src[i * 6 + 4] << 8) + src[i * 6 + 5];

			dst[i * 4 + 0] = (uint8_t)(r >> 8);
			dst[i * 4 + 1] = (uint8_t)(g >> 8);
			dst[i * 4 + 2] = (uint8_t)(b >> 8);
			dst[i * 4 + 3] = ((r == tr) && (g == tg) && (b == tb)) ? 0 : 255;
		}
	}
	else
	{
		while (pixels--)
		{
			dst[0] = src[0];
			dst[1] = src[2];
			dst[2] = src[4];
			dst[3] = 255;
			
			dst += 4;
			src += 6;
		}
	}

	return TRUE;
}

static int unpack_grey_alpha_8(uint8_t* src, uint8_t* dst, int pixels, png_decoder* pwi)
{
	(void)pwi;
	while (pixels--)
	{
		dst[0] = src[0];
		dst[1] = src[1];
		dst += 2;
		src += 2;
	}

	return TRUE;
}

static int unpack_grey_alpha_16(uint8_t* src, uint8_t* dst, int pixels, png_decoder* pwi)
{
	(void)pwi;
	while (pixels--)
	{
		dst[0] = src[0];
		dst[1] = src[2];
		dst += 2;
		src += 4;
	}

	return TRUE;
}

static int unpack_true_alpha_8(uint8_t* src, uint8_t* dst, int pixels, png_decoder* pwi)
{
	(void)src;
	(void)dst;
	(void)pixels;
	(void)pwi;
	return FALSE;
}

static int unpack_true_alpha_16(uint8_t* src, uint8_t* dst, int pixels, png_decoder* pwi)
{
	(void)pwi;
	while (pixels--)
	{
		dst[0] = src[0];
		dst[1] = src[2];
		dst[2] = src[4];
		dst[3] = src[6];
		dst += 4;
		src += 8;
	}

	return TRUE;
}

void png_decoder::unpredict_sub(uint8_t* lst, uint8_t* cur, uint32_t bytes, int bpp)
{
	(void)lst;
	if (bytes == (uint32_t)bpp)
		return;

	cur += bpp;
	bytes -= bpp;

	while (bytes--)
	{
		*cur += *(cur - bpp);
		cur++;
	}
}

void png_decoder::unpredict_up(uint8_t* lst, uint8_t* cur, uint32_t bytes, int bpp)
{
	(void)bpp;
	while (bytes--)
		*cur++ += *lst++;
}

void png_decoder::unpredict_average(uint8_t* lst, uint8_t* cur, uint32_t bytes, int bpp)
{
	int i;

	for (i = 0; i < bpp; i++)
		*cur++ += (*lst++ >> 1);

	if (bytes == (uint32_t)bpp)
		return;

	bytes -= bpp;

	while (bytes--)
	{
		*cur += ((*lst++ + *(cur - bpp)) >> 1);
		cur++;
	}
}

inline uint8_t png_decoder::paeth_predictor(int a, int b, int c)
{
	int p, pa, pb, pc;