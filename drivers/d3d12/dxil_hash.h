#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void compute_dxil_hash(const BYTE *pData, UINT byteCount, BYTE *pOutHash);
